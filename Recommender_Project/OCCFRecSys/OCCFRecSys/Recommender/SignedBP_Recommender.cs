using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class SignedBP_Recommender : Recommender
    {
        public int fold;
        string candidate_item;

        // BP variables        
        BPMatrix edge_list;
        public double propagation_alpha;
        public int num_iter, max_user_id, max_item_id;
        Dictionary<int, List<Score>> recommend_user_item;
        Dictionary<int, NodePotential> node_potential_like, node_potential_dislike;
        double[,] propagation_matrix_like = new double[2, 2] { { 0.5, 0.5 }, { 0.5, 0.5 } };
        double[,] propagation_matrix_dislike = new double[2, 2] { { 0.5, 0.5 }, { 0.5, 0.5 } };
        //    L D 
        //  L - - 
        //  D - -   <- 이 형태의 Propagation Matrix

        // 메모리 문제일 때, 각 정확도 저장
        bool local_check; // 사용자 별 정확도 저장 유무
        double[] recall, prec, ndcg;
        double mrr = 0d;

        public SignedBP_Recommender(string candidate_item, int num_iter, int num_recommend, double propagation_alpha, int fold, StreamReader training, StreamReader test)
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
            for (int i = 0; i < 2; i++)
            {
                propagation_matrix_like[i, i] += propagation_alpha;
                if (i == 0)
                    propagation_matrix_like[i, i + 1] -= propagation_alpha;
                else
                    propagation_matrix_like[i, i - 1] -= propagation_alpha;
            }
            Console.WriteLine(propagation_matrix_like[0, 0] + " " + propagation_matrix_like[0, 1] + " " + propagation_matrix_like[1, 0] + " " + propagation_matrix_like[1, 1]);

            for (int i = 0; i < 2; i++)
            {
                propagation_matrix_dislike[i, i] -= propagation_alpha;
                if (i == 0)
                    propagation_matrix_dislike[i, i + 1] += propagation_alpha;
                else
                    propagation_matrix_dislike[i, i - 1] += propagation_alpha;
            }
            Console.WriteLine(propagation_matrix_dislike[0, 0] + " " + propagation_matrix_dislike[0, 1] + " " + propagation_matrix_dislike[1, 0] + " " + propagation_matrix_dislike[1, 1]);
            
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

        public override void ReadTrainingSet(StreamReader sr)
        {
            max_user_id = int.MinValue;
            max_item_id = int.MinValue;

            edge_list = new BPMatrix();
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
                    forward_edge.SetLabel(true);
                    edge_list.SetAt(user_id, item_id, forward_edge);

                    // 역방향 edge
                    Edge backward_edge = new Edge();
                    backward_edge.SetLabel(true);
                    edge_list.SetAt(item_id, user_id, backward_edge);

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
                    forward_edge.SetLabel(false);
                    edge_list.SetAt(user_id, item_id, forward_edge);

                    // 역방향 edge
                    Edge backward_edge = new Edge();
                    backward_edge.SetLabel(false);
                    edge_list.SetAt(item_id, user_id, backward_edge);

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
            SignedBeliefPropagation BP = new SignedBeliefPropagation(edge_list, node_potential_like, node_potential_dislike, propagation_matrix_like, propagation_matrix_dislike, max_user_id, max_item_id);
            recommend_user_item = new Dictionary<int, List<Score>>();

            // BP 수행
            int num = 0;            
            foreach (int target_user in correct_user_item.Keys)
            {
                if (!node_potential_like.ContainsKey(target_user)) continue;
                // target user 마다 다시 message passing을 수행하기 위해 점수 초기화
                if (num != 0)
                    edge_list.Clear();

                List<Score> recommend_items = new List<Score>();
                recommend_items = PerformBP(BP, target_user, num);

                recommend_user_item.Add(target_user, recommend_items);
                num++;

                if (num % 50 == 0)
                    Console.WriteLine("Compute BP score for {0} user / {1} users (fold{2}): Complete", num, correct_user_item.Count(), fold);
            }
            Console.WriteLine("Compute BP scores: Complete");
        }

        public List<Score> PerformBP(SignedBeliefPropagation BP, int target_user, int num)
        {
            List<Score> belief_score_list = new List<Score>();
            BP.AssignNodePotential(target_user);

            for (int iter = 0; iter < num_iter; iter++)
            {
                // message passing: target user마다 독립적으로 수행됨
                BP.PassMessage(target_user, iter);
                // BP 점수 계산
                belief_score_list = BP.ComputeBeliefScore(target_user, test_items, num_recommend, correct_user_item);                
            }

            List<Score> recommend_items = new List<Score>();
            foreach (Score items in belief_score_list)
            {
                if (Program.remove_items != null)
                    if (Program.remove_items[target_user].Contains(items.item_id)) continue;

                //if (candidate_item == "Longtail_items")
                //    if (Program.longtails.Contains(items.item_id)) continue;

                if (node_potential_like[target_user].linkednode_list.Contains(items.item_id)) continue;

                //if (node_potential_dislike.ContainsKey(target_user))
                //    if (node_potential_dislike[target_user].linkednode_list.Contains(items.item_id)) continue;
                
                if (recommend_items.Count < num_recommend)
                {
                    Score user_score = new Score(items.item_id, items.score);
                    recommend_items.Add(user_score);
                }
                else
                    break;           
            }

            // 메모리 부족으로 이미 BP 수행한 target user 관련 데이터 지움
            node_potential_like.Remove(target_user);
            //node_potential_dislike.Remove(target_user);
            BP.Clean(target_user);

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
