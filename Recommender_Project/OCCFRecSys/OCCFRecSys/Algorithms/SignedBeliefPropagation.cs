using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class SignedBeliefPropagation
    {
        int max_user_id;
        int max_item_id;
        BPMatrix edge_list;
        double[,] propagation_matrix;
        double[,] propagation_matrix_like;
        double[,] propagation_matrix_dislike;
        Dictionary<int, NodePotential> node_potential_like;
        Dictionary<int, NodePotential> node_potential_dislike;

        public SignedBeliefPropagation
            (BPMatrix edge_list, Dictionary<int, NodePotential> node_potential_like, Dictionary<int, NodePotential> node_potential_dislike, double[,] propagation_matrix_like, double[,] propagation_matrix_dislike, int max_user_id, int max_item_id)
        {
            this.edge_list = edge_list;
            this.node_potential_like = node_potential_like;
            this.node_potential_dislike = node_potential_dislike;
            this.propagation_matrix_like = propagation_matrix_like;
            this.propagation_matrix_dislike = propagation_matrix_dislike;
            this.max_user_id = max_user_id;
            this.max_item_id = max_item_id;
        }

        public void AssignNodePotential(int node_id)
        {
            // 그래프의 모든 노드들에 대해서 target user가 다른 노드에 link가 있으면 그 다른 노드의 like potential은 0.9, 없으면 0.5
            foreach (KeyValuePair<int, Dictionary<int, Edge>> row in edge_list.bp_matrix)
            {
                if (node_potential_like[node_id].linkednode_list.Contains(row.Key))
                    node_potential_like[node_id].SetPriorScore(row.Key, 0.9);
                else if (row.Key == node_id)
                    node_potential_like[node_id].SetPriorScore(row.Key, 0.9);
                else if (node_potential_dislike.ContainsKey(node_id))
                {
                    if (node_potential_dislike[node_id].linkednode_list.Contains(row.Key))
                        node_potential_like[node_id].SetPriorScore(row.Key, 0.4);
                    else
                        node_potential_like[node_id].SetPriorScore(row.Key, 0.5);
                }
                else
                    node_potential_like[node_id].SetPriorScore(row.Key, 0.5);
            }
        }

        /// <summary>
        /// target user를 기반으로, 전체 그래프의 모든 edge의 message score 계산
        /// </summary>
        /// <param name="node_id"></param>
        /// <param name="num_iter"></param>
        public void PassMessage(int node_id, int num_iter)
        {
            // target user를 기반으로 그래프 상의 모든 노드 쌍의 edge에 대해 루프
            foreach (KeyValuePair<int, Dictionary<int, Edge>> outerEdge in edge_list.bp_matrix)
            {
                double sumMessage_Like = 1d;
                double sumMessage_Dislike = 1d;

                int mc = 0;
                // outerEdge랑 연결된 모든 에지로부터 들어오는 메시지를 곱해둠
                foreach (KeyValuePair<int, Edge> innerEdge in outerEdge.Value)
                {
                    sumMessage_Like = sumMessage_Like * edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Like;
                    sumMessage_Dislike = sumMessage_Dislike * edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Dislike;
                    mc++;

                    CheckNum(sumMessage_Like, sumMessage_Dislike);
                    // 받는 message가 많은 경우, 그 message 점수들을 모두 곱할 때 발생하는 underflow를 피하기 위한 처리
                    if (num_iter > 0)
                    {
                        if (mc % 20 == 0)
                        {
                            sumMessage_Like *= 1000;
                            sumMessage_Dislike *= 1000;
                        }
                    }
                    CheckNum(sumMessage_Like, sumMessage_Dislike);
                }

                foreach (KeyValuePair<int, Edge> innerEdge in outerEdge.Value)
                {
                    double Message_Like = 0d;
                    double Message_Dislike = 0d;
                    // 위에서 곱해놓은 메시지에서 지금 메시지를 보내고자 하는 노드로부터 들어온 메시지를 제거(나누기)한 후 업데이트
                    if (innerEdge.Value.GetLabel() == true)
                        propagation_matrix = propagation_matrix_like;
                    else
                        propagation_matrix = propagation_matrix_dislike;

                    Message_Like
                        = (node_potential_like[node_id].GetPriorScore(outerEdge.Key) * propagation_matrix[0, 0] * (sumMessage_Like / edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Like))
                        + ((1 - node_potential_like[node_id].GetPriorScore(outerEdge.Key)) * propagation_matrix[1, 0] * (sumMessage_Dislike / edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Dislike));
                    Message_Dislike
                        = (node_potential_like[node_id].GetPriorScore(outerEdge.Key) * propagation_matrix[0, 1] * (sumMessage_Like / edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Like))
                        + ((1 - node_potential_like[node_id].GetPriorScore(outerEdge.Key)) * propagation_matrix[1, 1] * (sumMessage_Dislike / edge_list.bp_matrix[innerEdge.Key][outerEdge.Key].PreviousMessage_Dislike));
                    
                    // Normalization 하는 버젼
                    edge_list.bp_matrix[outerEdge.Key][innerEdge.Key].Message_Like = Message_Like / (Message_Like + Message_Dislike);
                    edge_list.bp_matrix[outerEdge.Key][innerEdge.Key].Message_Dislike = 1 - (Message_Like / (Message_Like + Message_Dislike));

                    // Normalization 하지 않는 버젼
                    //edge_list.bp_matrix[outerEdge.Key][innerEdge.Key].Message_Like = Message_Like;
                    //edge_list.bp_matrix[outerEdge.Key][innerEdge.Key].Message_Dislike = Message_Dislike;

                    CheckNum(sumMessage_Like, sumMessage_Dislike);
                }
            }

            // 새로 바뀐 message를 다음 단계 계산에 사용하기 위해 previous에 넣어주는 부분
            foreach (KeyValuePair<int, Dictionary<int, Edge>> row in edge_list.bp_matrix)
            {
                foreach (KeyValuePair<int, Edge> col in row.Value)
                {
                    edge_list.bp_matrix[row.Key][col.Key].PreviousMessage_Like = edge_list.bp_matrix[row.Key][col.Key].Message_Like;
                    edge_list.bp_matrix[row.Key][col.Key].PreviousMessage_Dislike = edge_list.bp_matrix[row.Key][col.Key].Message_Dislike;                    
                }
            }
        }

        /// <summary>
        /// 타겟 사용자에 대해 모든 아이템들에 대한 추천 점수 계산
        /// </summary>
        /// <param name="node_id"></param>
        /// <param name="test_items"></param>
        /// <param name="num_recommend"></param>
        /// <param name="correct_user_item"></param>
        /// <returns></returns>
        public List<Score> ComputeBeliefScore(int node_id, HashSet<int> items, int num_recommend, Dictionary<int, HashSet<int>> correct_user_item)
        {
            HashSet<Score> temp_belief_score_list = new HashSet<Score>();
            List<Score> belief_score_list = new List<Score>();
            Score scores;

            // 점수 계산하면서 추천 리스트 생성
            for (int i = max_user_id + 1; i <= max_item_id + 1; i++)
            {
                //if (BP_Recommender.longtails.Contains(i)) continue;

                // score 계산
                double score_like = Compute(node_id, i);
                scores = new Score(i, score_like);

                temp_belief_score_list.Add(scores);

                //if (temp_belief_score_list.Count < num_recommend)
                //    temp_belief_score_list.Add(scores);
                //else
                //{
                //    double min = 1d;
                //    int remove_id = 0;
                //    foreach (Score s in temp_belief_score_list)
                //        if (s.score < min)
                //        {
                //            min = s.score;
                //            remove_id = s.item_id;
                //        }
                //    if (score_like <= min) continue;
                //    else
                //    {
                //        Score remove_score = new Score(remove_id, min);
                //        temp_belief_score_list.Remove(remove_score);
                //        temp_belief_score_list.Add(scores);
                //    }
                //}
            }

            // belief score 정렬
            belief_score_list = temp_belief_score_list.ToList();
            belief_score_list.Sort(Score.CompareScore);

            //Console.WriteLine();
            //// 정렬된 belief score 중 정답 아이템의 추천 점수와 동일한 점수를 가진 아이템들 중 가장 우선순위로 옮김
            //for (int i = 0; i < belief_score_list.Length; i++)
            //{
            //    for (int j = 0; j < num_recommend; j++)
            //        for()
            //        if (belief_score_list[i][j].score <= target_score_like)
            //        {
            //            //Console.WriteLine(i + " " + j);
            //            belief_score_list[i][j].item_id = target_item;
            //            belief_score_list[i][j].score = target_score_like;
            //            break;
            //        }
            //}

            return belief_score_list;
        }

        /// <summary>
        /// target item에 대한 target user의 belief score를 계산  (target item 점수 계산 방법: 해당 아이템에게 들어오는 메시지 점수 곱 * target item의 potential score)
        /// </summary>
        /// <param name="target_user"></param>
        /// <param name="target_item"></param>
        /// <returns></returns>
        public double Compute(int target_user, int target_item)
        {
            double sumMessage_Like = 1d;
            double sumMessage_Dislike = 1d;

            double score_like = 0d;
            // edge가 아예 없는 items 제외
            if (edge_list.bp_matrix.ContainsKey(target_item))
            {
                // target item으로 향하는 message들을 읽어옴
                int mc = 0;
                foreach (KeyValuePair<int, Edge> temp_edge in edge_list.bp_matrix[target_item])
                {
                    sumMessage_Like = sumMessage_Like * edge_list.bp_matrix[temp_edge.Key][target_item].Message_Like;
                    sumMessage_Dislike = sumMessage_Dislike * edge_list.bp_matrix[temp_edge.Key][target_item].Message_Dislike;
                    mc++;

                    CheckNum(sumMessage_Like, sumMessage_Dislike);
                    // 받는 message가 많은 경우, 그 message 점수들을 모두 곱할 때 발생하는 underflow를 피하기 위한 처리
                    if (mc % 20 == 0)
                    {
                        sumMessage_Like *= 1000;
                        sumMessage_Dislike *= 1000;
                    }
                    CheckNum(sumMessage_Like, sumMessage_Dislike);
                }
                // 해당 item이 받은 message들을 통해 취합한 점수와 해당 item의 potential을 곱해줌
                double tempScoreLike = node_potential_like[target_user].GetPriorScore(target_item) * sumMessage_Like;
                double tempScoreDislike = (1 - node_potential_like[target_user].GetPriorScore(target_item)) * sumMessage_Dislike;
                // normalization
                score_like = tempScoreLike / (tempScoreLike + tempScoreDislike);
                //double ScoreDisike = (1 - ScoreLike);       
            }

            return score_like;
        }

        public void CheckNum(double like, double dislike)
        {
            if (double.IsNaN(like) || double.IsInfinity(like))
                Program.log.WriteLine("Error: Message Like " + like);
            if (double.IsNaN(dislike) || double.IsInfinity(dislike))
                Program.log.WriteLine("Error: Message Dislike " + dislike);
        }

        public void Clean(int node_id)
        {
            node_potential_like.Remove(node_id);
        }
    }
}
