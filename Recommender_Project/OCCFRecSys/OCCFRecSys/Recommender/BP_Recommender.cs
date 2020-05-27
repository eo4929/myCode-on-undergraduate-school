using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class BP_Recommender : Recommender
    {         
        public int fold;
        string candidate_item;
        Dictionary<int, List<Score>> recommend_user_item;
        // BP variables
        BPMatrix edge_list;        
        public double propagation_alpha;
        Dictionary<int, NodePotential> node_potential;
        public int num_iter, max_user_id, max_item_id;
        double[,] propagation_matrix = new double[2, 2] { { 0.5, 0.5 }, { 0.5, 0.5 } };
        //    L D 
        //  L - - 
        //  D - -   <- 이 형태의 Propagation Matrix

        // 메모리 문제일 때, 각 정확도 저장
        StreamWriter results_output;
        double[] recall, prec, ndcg;
        double mrr = 0d;

        public BP_Recommender(string candidate_item, int num_iter, int num_recommend, double propagation_alpha, int fold, StreamReader training, StreamReader test)
            : this(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test, null)
        {
        }

        /// <summary>
        /// Belief propagation을 이용하여 추천한 후, 정확도 측정
        /// </summary>
        /// <param name="target"></param>
        /// <param name="file_path">training, test set이 불러오고 결과를 출력할 파일 경로</param>
        /// <param name="num_iter">반복 수행 횟수</param>
        public BP_Recommender(string candidate_item, int num_iter, int num_recommend, double propagation_alpha, int fold, StreamReader training, StreamReader test, StreamWriter results_output)
        {
            this.candidate_item = candidate_item;
            this.num_iter = num_iter;
            this.fold = fold;
            this.propagation_alpha = propagation_alpha;
            this.results_output = results_output;

            Init(num_recommend, training, test);            
        }

        public override void Init(int num_recommend, StreamReader training, StreamReader test)
        {
            this.num_recommend = num_recommend;
            for (int i = 0; i < 2; i++)
            {
                propagation_matrix[i, i] += propagation_alpha;
                if (i == 0)
                    propagation_matrix[i, i + 1] -= propagation_alpha;
                else
                    propagation_matrix[i, i - 1] -= propagation_alpha;
            }
            Console.WriteLine(propagation_matrix[0, 0] + " " + propagation_matrix[0, 1] + " " + propagation_matrix[1, 0] + " " + propagation_matrix[1, 1]);

            #region test set 읽기
            ReadTestSet(test, candidate_item);
            test.Close();
            Console.WriteLine("Read test set: Complete");
            #endregion

            #region training set 읽기
            // 모든 edges 정보와 potential 세팅
            ReadTrainingSet(training);
            training.Close();
            Console.WriteLine("Read train set: Complete");
            #endregion
        }

        /// <summary>
        /// graph의 노드와 엣지를 읽고, 각 노드의 node potential 초기화
        /// </summary>
        /// <param name="sr">graph 파일 경로</param>
        public override void ReadTrainingSet(StreamReader sr)
        {
            max_user_id = int.MinValue;
            max_item_id = int.MinValue;

            edge_list = new BPMatrix();
            node_potential = new Dictionary<int, NodePotential>();

            while (sr.EndOfStream == false)
            {
                line = sr.ReadLine().Split(new char[] { '\t', ' ' }, StringSplitOptions.RemoveEmptyEntries);
                int user_id = int.Parse(line[0].ToString());
                int item_id = int.Parse(line[1].ToString()) + Program.num_user;

                // edge 저장 (메시지에 대한 이전 message_like,dislike 점수와 현재 message_like,dislike 점수 저장)
                // 정방향 edge
                Edge forward_edge = new Edge();
                edge_list.SetAt(user_id, item_id, forward_edge);
                // 역방향 edge
                Edge backward_edge = new Edge();
                edge_list.SetAt(item_id, user_id, backward_edge);

                // user, item 개수 확인
                if (max_user_id < user_id)
                    max_user_id = user_id;
                if (max_item_id < item_id)
                    max_item_id = item_id;

                if (node_potential.ContainsKey(user_id))
                    node_potential[user_id].AddNode(item_id);
                else
                {
                    NodePotential temp_node = new NodePotential();
                    temp_node.AddNode(item_id);
                    node_potential.Add(user_id, temp_node);
                }
            }
        }

        public override void Recommend()
        {
            // BP 기본 세팅
            BeliefPropagation BP = new BeliefPropagation(edge_list, node_potential, propagation_matrix, max_user_id, max_item_id);
            recommend_user_item = new Dictionary<int, List<Score>>();

            // BP 수행
            int num = 0;
            foreach (int target_user in correct_user_item.Keys)
            {
                if (!node_potential.ContainsKey(target_user)) continue;

                List<Score> recommend_items = new List<Score>(); 
                recommend_items = PerformBP(BP, target_user, num, 0.9, results_output);

                recommend_user_item.Add(target_user, recommend_items);

                num++;

                if (num % 50 == 0)
                    Console.WriteLine("Compute BP score for {0} user / {1} users (fold{2}): Complete", num, correct_user_item.Count(), fold);
            }
            if(results_output != null)
                results_output.Close();
            Console.WriteLine("Compute BP scores: Complete");
        }

        /// <summary>
        /// Belief propagation 수행
        /// </summary>
        /// <param name="BP">Belief propagation 클래스</param>
        /// <param name="num_iter">반복 횟수</param>
        /// <param name="alpha">propagation matrix의 alpha</param>
        public List<Score> PerformBP(BeliefPropagation BP, int target_user, int num, double target_prior, StreamWriter results_output)
        {
            // target user 마다 다시 message passing을 수행하기 위해 점수 초기화
            if (num != 0)
                edge_list.Clear();

            List<Score> belief_score_list = new List<Score>();
            BP.AssignNodePotential(target_user, target_prior);

            for (int iter = 0; iter < num_iter; iter++)
            {
                // message passing: target user마다 독립적으로 수행됨
                BP.PassMessage(target_user, iter);

                // BP 점수 계산
                belief_score_list = BP.ComputeBeliefScore(target_user, test_items, num_recommend, correct_user_item);
            }           

            List<Score> recommend_items = new List<Score>();
            int rec = 0;
            for (int i = 0; i < belief_score_list.Count; i++)
            {
                if (node_potential[target_user].linkednode_list.Contains(belief_score_list[i].item_id)) continue;
                if (candidate_item == "Longtail_items")
                    if (Program.longtails.Contains(belief_score_list[i].item_id)) continue;

                if (results_output != null)
                {
                    rec++;
                    if (rec <= num_recommend)
                        results_output.WriteLine(target_user + "\t" + belief_score_list[i].item_id + "\t" + belief_score_list[i].score);
                    else
                        break;
                }
                else
                {
                    if (recommend_items.Count < num_recommend)
                    {
                        Score user_score = new Score(belief_score_list[i].item_id, belief_score_list[i].score);
                        recommend_items.Add(user_score);
                    }
                    else
                        break;
                }
            }
            //results_output.Flush();

            // 메모리 부족으로 이미 BP 수행한 target user 관련 데이터 지움
            node_potential.Remove(target_user);
            //BP.Clean(target_user);

            #region 중간 체크
            //if (num % 10 == 0)
            //{
            //    StreamWriter temp1 = new StreamWriter(file_path + "\\Results\\User_" + target + "_BP(" + candidate_items[0] + ")" + num + ".results");
            //    StreamWriter temp2 = new StreamWriter(file_path + "\\Results\\User_" + target + "_BP(" + candidate_items[1] + ")" + num + ".results");
            //    for (int i = 0; i < num_iter; i++)
            //    {
            //        temp1.WriteLine("Results for iteration {0}", (i + 1));
            //        for (int j = 0; j < topks.Length; j++)
            //            temp1.WriteLine("Top@{0}\t{1}", topks[j], test_items_hit[i, j]);
            //    }
            //    temp1.Flush();
            //    temp1.Close();
            //    for (int i = 0; i < num_iter; i++)
            //    {
            //        temp2.WriteLine("Results for iteration {0}", (i + 1));
            //        for (int j = 0; j < topks.Length; j++)
            //            temp2.WriteLine("Top@{0}\t{1}", topks[j], all_items_hit[i, j]);
            //    }
            //    temp2.Flush();
            //    temp2.Close();
            //}
            #endregion            

            return recommend_items;
        }

        public override void PrintResults(StreamWriter sw)
        {
            ComputeMRRatN(recommend_user_item, sw);
            ComputeAUC(recommend_user_item, sw);
            ComputeHLU(recommend_user_item, sw);
            ComputeAccuary(recommend_user_item, sw);
            ComputeNDCG(recommend_user_item, sw);
            Console.WriteLine("Compute accuracy of recommendation: Complete");
        }
    }
}
