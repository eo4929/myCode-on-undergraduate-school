using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OCCFRecSys
{
    public class NodePotential
    {
        public HashSet<int> linkednode_list = new HashSet<int>();                       // user 혹은 item에 연결된 nodes의 list
        public Dictionary<int, double> prior_score = new Dictionary<int, double>();     // user 혹은 item이 연결된 nodes에 대한 prior score 저장

        /// <summary>
        /// 해당 user와 연결된 item 혹은 해당 item과 연결된 user의 리스트 저장
        /// </summary>
        /// <param name="node_id"></param>
        /// <returns></returns>
        public bool AddNode(int node_id)
        {
            if (linkednode_list.Contains(node_id))
                return false;
            else
            {
                linkednode_list.Add(node_id);
                return true;
            }
        }

        /// <summary>
        /// 해당 node의 prior score 설정
        /// </summary>
        public void SetPriorScore(int node_id, double score)
        {
            if (!prior_score.ContainsKey(node_id))
                prior_score.Add(node_id, score);
        }

        /// <summary>
        /// 해당 node의 prior score 확인
        /// </summary>
        public double GetPriorScore(int node_id)
        {
            return prior_score[node_id];
        }

        public bool haveNode(int node_id)
        {
            return linkednode_list.Contains(node_id);
        }

    }
}
