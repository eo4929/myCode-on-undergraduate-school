using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class BalanceBP_Recommender : Recommender
    {
        public int fold;
        string candidate_item;
        Dictionary<int, List<Score>> recommend_user_item;

        // BP variables
        public double propagation_alpha;
        public int num_iter, max_user_id, max_item_id;
        BPMatrix like_edges, dislike_edges;
        Dictionary<int, NodePotential> node_potential_like, node_potential_dislike;
        double[,] propagation_matrix = new double[2, 2] { { 0.5, 0.5 }, { 0.5, 0.5 } };
        //    L D 
        //  L - - 
        //  D - -   <- 이 형태의 Propagation Matrix

        public BalanceBP_Recommender(string candidate_item, int num_iter, int num_recommend, double propagation_alpha, int fold, StreamReader training, StreamReader test) //, double weight
        {
            this.candidate_item = candidate_item;
            this.num_iter = num_iter;
            this.fold = fold;
            this.propagation_alpha = propagation_alpha;

            Init(num_recommend, training, test);
        }
        
        public override void Init(int num_recommend, StreamReader training, StreamReader test)
        {
            this.num_recommend = num_recommend;
            // propagation matrix 세팅
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

            like_edges = new BPMatrix();
            dislike_edges = new BPMatrix();
            node_potential_like = new Dictionary<int, NodePotential>();
            node_potential_dislike = new Dictionary<int, NodePotential>();

            while (sr.EndOfStream == false)
            {
                line = sr.ReadLine().Split(new char[] { '\t', ' ' }, StringSplitOptions.RemoveEmptyEntries);
                int user_id = int.Parse(line[0].ToString());
                int item_id = int.Parse(line[1].ToString()) + Program.num_user;
                int rating = int.Parse(line[2].ToString());

                // user, item 개수 확인
                if (max_user_id < user_id)
                    max_user_id = user_id;
                if (max_item_id < item_id)
                    max_item_id = item_id;

                if (rating == 1)
                {
                    // edge 저장 (메시지에 대한 이전 message_like,dislike 점수와 현재 message_like,dislike 점수 저장)
                    // 정방향 edge
                    Edge forward_edge = new Edge();
                    like_edges.SetAt(user_id, item_id, forward_edge);
                    // 역방향 edge
                    Edge backward_edge = new Edge();
                    like_edges.SetAt(item_id, user_id, backward_edge);

                    // 각 사용자의 like items 저장
                    if (node_potential_like.ContainsKey(user_id))
                        node_potential_like[user_id].AddNode(item_id);
                    else
                    {
                        NodePotential temp_node = new NodePotential();
                        temp_node.AddNode(item_id);
                        node_potential_like.Add(user_id, temp_node);
                    }
                }
                else
                {
                    // edge 저장 (메시지에 대한 이전 message_like,dislike 점수와 현재 message_like,dislike 점수 저장)
                    // 정방향 edge
                    Edge forward_edge = new Edge();
                    dislike_edges.SetAt(user_id, item_id, forward_edge);
                    // 역방향 edge
                    Edge backward_edge = new Edge();
                    dislike_edges.SetAt(item_id, user_id, backward_edge);

                    // 각 사용자의 dislike items 저장
                    if (node_potential_dislike.ContainsKey(user_id))
                        node_potential_dislike[user_id].AddNode(item_id);
                    else
                    {
                        NodePotential temp_node = new NodePotential();
                        temp_node.AddNode(item_id);
                        node_potential_dislike.Add(user_id, temp_node);
                    }
                }
            }
        }

        public override void Recommend()
        {
            // BP 기본 세팅
            BeliefPropagation BP_like = new BeliefPropagation(like_edges, node_potential_like, propagation_matrix, max_user_id, max_item_id);
            BeliefPropagation BP_dislike = new BeliefPropagation(dislike_edges, node_potential_dislike, propagation_matrix, max_user_id, max_item_id);
            recommend_user_item = new Dictionary<int, List<Score>>();

            // BP 수행
            int num = 0;
            foreach (int target_user in correct_user_item.Keys)
            {
                if (!node_potential_like.ContainsKey(target_user) && !node_potential_dislike.ContainsKey(target_user)) continue;

                // target user 마다 다시 message passing을 수행하기 위해 점수 초기화
                if (num != 0)
                {
                    like_edges.Clear();
                    dislike_edges.Clear();
                }

                Dictionary<int, double> like_items = PerformBP(BP_like, target_user, 0.9);
                Dictionary<int, double> dislike_items = PerformBP(BP_dislike, target_user, 0.7);

                double dislike_min = 1d;
                foreach (int item_id in dislike_items.Keys)
                {
                    if (dislike_items[item_id] == 0) continue;
                    if (dislike_min > dislike_items[item_id])
                        dislike_min = dislike_items[item_id];
                }

                List<Score> recommend_items = new List<Score>();
                foreach (int item_id in like_items.Keys)
                {
                    double final = 0d;
                    if (Program.remove_items != null)
                        if (Program.remove_items[target_user].Contains(item_id)) continue;

                    //if (candidate_item == "Longtail_items")
                    //    if (Program.longtails.Contains(item_id)) continue;

                    if (node_potential_like[target_user].linkednode_list.Contains(item_id)) continue;
                    else if (node_potential_dislike.ContainsKey(target_user))
                        if (node_potential_dislike[target_user].linkednode_list.Contains(item_id)) continue;

                    if (like_items[item_id] == 0) continue;
                    if (dislike_items[item_id] == 0)
                        final = like_items[item_id] - dislike_min;
                    else
                        final = like_items[item_id] - dislike_items[item_id];

                    Score user_score = new Score(item_id, final);
                    recommend_items.Add(user_score);
                }

                recommend_items.Sort(Score.CompareScore);                
                recommend_user_item.Add(target_user, recommend_items);

                num++;
                if (num % 50 == 0)
                    Console.WriteLine("Compute BP score for {0} user / {1} users (fold{2}): Complete", num, correct_user_item.Count(), fold);
            }
            Console.WriteLine("Compute BP scores: Complete");
        }

        /// <summary>
        /// Belief propagation 수행
        /// </summary>
        /// <param name="BP">Belief propagation 클래스</param>
        /// <param name="num_iter">반복 횟수</param>
        /// <param name="alpha">propagation matrix의 alpha</param>
        public Dictionary<int, double> PerformBP(BeliefPropagation BP, int target_user, double target_prior)
        {
            List<Score> belief_score_list = new List<Score>();
            BP.AssignNodePotential(target_user, target_prior);

            for (int iter = 0; iter < num_iter; iter++)
            {
                // message passing: target user마다 독립적으로 수행됨
                BP.PassMessage(target_user, iter);

                // BP 점수 계산
                belief_score_list = BP.ComputeBeliefScore(target_user, test_items, num_recommend, correct_user_item);
            }

            Dictionary<int, double> recommend_items = new Dictionary<int, double>();
            // 아이템 별 최종 점수 저장
            for (int i = 0; i < belief_score_list.Count; i++)
                recommend_items.Add(belief_score_list[i].item_id, belief_score_list[i].score);

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
