using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OCCFRecSys
{
    public class RWR
    {
        private double[] score;
        private double D;
        private double T = -1.0d;
        private int I = 10;
        private WeightedUndirectedGraph graph;
        private double[] restart;

        /// <param name="TargetGraph">
        /// Undirected web graph where the RWR will be calculated.
        /// </param>
        /// <param name="DampingFactor">
        /// The damping factor alpha.
        /// </param>
        /// <param name="InitialNode">
        /// List of initial nodes whose initial score set to be 1 and all others 0.
        /// </param>
        public RWR(WeightedUndirectedGraph TargetGraph, double DampingFactor, int InitialNode)
            : this(TargetGraph, DampingFactor, InitialNode, -1)
        {
        }

        /// <param name="TargetGraph">
        /// Undirected web graph where the RWR will be calculated.
        /// </param>
        /// <param name="DampingFactor">
        /// The damping factor alpha.
        /// </param>
        /// <param name="InitialNode">
        /// List of initial nodes whose initial score set to be 1 and all others 0.
        /// </param>
        /// <param name="T">
        /// Error tolerance.
        /// </param>
        public RWR(WeightedUndirectedGraph TargetGraph, double DampingFactor, int InitialNode, double T)
        {
            this.D = DampingFactor;
            this.graph = TargetGraph;
            this.T = T;

            score = new double[graph.Size];
            restart = new double[graph.Size];
            for (int i = 0; i < score.Length; i++)
            {
                if (InitialNode == i)
                {
                    score[i] = 1.0d;
                    restart[i] = 1.0d;
                }
                else
                {
                    score[i] = 0.0d;
                    restart[i] = 0.0d;
                }
            }
        }

        public List<Weight> Calculate()
        {
            double stderr = double.MaxValue;
            int cnt = 1;

            //Console.WriteLine("Start: RWR...");

            List<Weight> result = new List<Weight>();

            if (T <= 0.0d)
                for (int i = 1; i <= I; i++)
                {
                    //Console.Write("Iteration {0}: ", i);
                    stderr = Compute();
                    //Console.Write(stderr);
                    //Console.WriteLine();
                }
            else
                while (stderr > T)
                {
                    //Console.Write("Iteration {0}: ", cnt++);
                    stderr = Compute();
                    //Console.Write(stderr);
                    //Console.WriteLine();
                }

            for (int i = 0; i < score.Length; i++)
                result.Add(new Weight(i, score[i]));
            result.Sort(Weight.CompareWeightDesc);

            return result;
        }

        private double Compute()
        {
            HashSet<Weight> links;
            Dictionary<int, double> dlink = new Dictionary<int, double>(); // 뎅글링링크 관련 데이터 저장
            double dscore = 0.0d; // 뎅글링링크에 의해 파급된 스코어 계
            double[] tempscore = new double[score.Length];
            double sum;

            // initialize temp score vector
            for (int i = 0; i < tempscore.Length; i++)
                tempscore[i] = 0.0d;

            for (int i = 0; i < score.Length; i++)
            {
                // get outlinks
                links = graph.GetOutlinks(i);

                // 아웃링크 없는 경우(뎅글링링크)
                if (links == null)
                {
                    double s = (double)(score[i] / (double)(score.Length - 1)); // 네트워크 전반에 파급시킬 값
                    dlink.Add(i, s); // dlink에 자신이 네트워크 전반에 파급시킨 값을 저장
                    dscore += s;
                    continue;
                }
                // 아웃링크를 따라 파급
                else
                {
                    sum = 0.0d;
                    // get sum of out-link weights
                    foreach (Weight j in links)
                        sum += j.w;
                    // give score
                    foreach (Weight j in links)
                        tempscore[j.id] += (double)(score[i] * (j.w / sum));
                }
            }

            // 계산결과 적용
            double stderr = 0, tmpscore;

            for (int i = 0; i < score.Length; i++)
            {
                if (dlink.ContainsKey(i)) // 뎅글링링크의 경우 dscore에서 자신의 파급값을 제한다
                    tmpscore = ((tempscore[i] + dscore - dlink[i]) * D) + (restart[i] * (1.0d - D));
                else
                    tmpscore = ((tempscore[i] + dscore) * D) + (restart[i] * (1.0d - D));
                stderr += Math.Pow(score[i] - tmpscore, 2);
                score[i] = tmpscore;
            }
            return stderr;
        }

        public void PrintScore()
        {
            for (int i = 0; i < score.Length; i++)
                //Console.WriteLine(i.ToString() + "\t" + score[i].ToString());

                return;
        }
    }
}
