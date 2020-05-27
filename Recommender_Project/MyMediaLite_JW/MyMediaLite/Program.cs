using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MyMediaLite.Data;
using MyMediaLite.Eval;
using MyMediaLite.IO;
using MyMediaLite.ItemRecommendation;
using System.Diagnostics;
using System.IO;

namespace MyMediaLite
{
    public enum Algorithm
    {
        ItemPopularity2,
        BPRMF,
        WRMF
    };

    class Program
    {
        public static string file_path;
        public static StreamReader sr;
        public static StreamWriter sw;
        public static IList<int> top_head_items;
        public static string dataset;
        //iter_로 fold for문 안에 for문 하나 더 추가, performalgorithm 함수에 iter_도 함께 넣어서 호출 - 지원
        public static void Main(string[] args)
        {
            CandidateItems[] candidate_mode = { CandidateItems.UNION };//CandidateItems.LONGTAIL
            Algorithm[] algorithms = { Algorithm.WRMF }; 

            #region 5-fold cv
            string[] datasets = { "watcha" }; 
            foreach (string dataset in datasets)
            {
                file_path = "C:\\Users\\eo492\\" + dataset + "\\";
                foreach (Algorithm algo in algorithms)
                {                    
                    foreach (CandidateItems mode in candidate_mode)
                    {
                        Dictionary<string, double> average = new Dictionary<string, double>();
                        for (int fold = 1; fold <= 1; fold++)
                        {
                            for (int ri = 0; ri < 3; ri++)
                            {
                   
                                for (int iter_ = 0; iter_ < 5; iter_++) // wrmf 할땐 이거 3이 아닌 5로 바꿔라 알파가 0~4니까
                                {
                                    string base_file = dataset + "-train-" + fold + ".txt";
                                    string test_file = dataset + "-test-" + fold + ".txt";
                                    var training_data = ItemData.Read(file_path + base_file);
                                    var test_data = ItemData.Read(file_path + test_file);

                                    if (mode == CandidateItems.LONGTAIL)
                                    {
                                        sr = new StreamReader(file_path + dataset + "-tophead-" + fold + ".txt");
                                        top_head_items = ReadTopHeadItems(sr);
                                        sr.Close();
                                    }

                                    // 알고리즘 수행 후 추천 정확도 반환
                                    Dictionary<string, double> results = PerformAlgorithm(algo, mode, training_data, test_data, ri, iter_);
                                    Console.WriteLine("u_" + fold + " Recommendation: Complete");

                                    // 추천 정확도 출력될 경로
                                    string output = file_path + dataset + "-" + fold + "(" + algo + "_" + mode + ").result";
                                    PrintResults(output, results, average);
                                }
                            }
                        }

                        //// 5 fold cv
                        //sw = new StreamWriter(file_path + dataset + "(" + algo + "_" + mode + ").result");
                        //foreach (string measure in average.Keys)
                        //    sw.WriteLine(measure + "\t" + (average[measure] / 5d));
                        //sw.Flush();
                        //sw.Close();
                    }
                }
            }
            #endregion            
        }
        //파라미터로 iter 1개 받도록 추가. 추후 더 추가 예정. selectposonlyfeedbackalgorithm에 it(iter_)를 함께 보냄 - 지원
        public static Dictionary<string, double> PerformAlgorithm(Algorithm algo, CandidateItems mode, IPosOnlyFeedback training_data, IPosOnlyFeedback test_data, int ri, int it)
        {
            var recommender = SelectPosOnlyFeedbackAlgorithm(algo,ri,it);
            recommender.Feedback= training_data;
            recommender.Train();

            // evlaute Top-N recommendation
            return Items2.Evaluate(recommender, test_data, training_data, null, null, mode);
        }
        //console에서 결과값들 볼 수 있게 writeline추가 - 지원
        public static void PrintResults(string output, Dictionary<string, double> results, Dictionary<string, double> average)
        {
            var positions = new int[] { 5, 10, 15, 20, 25, 30, 50 };

            sw = new StreamWriter(output);
            sw.WriteLine("ATOP\t" + results["ATOP"]);
            sw.WriteLine("MRR\t" + results["MRR"]);
            sw.WriteLine("AUC\t" + results["AUC"]);
            sw.WriteLine("HLU\t" + results["HLU"]);
            foreach (int pos in positions)
                sw.WriteLine("prec@" + pos + "\t" + results["prec@" + pos]);
            foreach (int pos in positions)
                sw.WriteLine("recall@" + pos + "\t" + results["recall@" + pos]);
            foreach (int pos in positions)
                sw.WriteLine("mrr@" + pos + "\t" + results["mrr@" + pos]);
            foreach (int pos in positions)
                sw.WriteLine("f1@" + pos + "\t" + (2 * (results["prec@" + pos] * results["recall@" + pos])) / (results["prec@" + pos] + results["recall@" + pos]));
            foreach (int pos in positions)
                sw.WriteLine("NDCG@" + pos + "\t" + results["NDCG@" + pos]);
            foreach (int pos in positions)
                if(pos == 5)
                    Console.WriteLine("NDCG@" + pos + "\t" + results["NDCG@" + pos]);
            sw.Flush();
            sw.Close();

            sr = new StreamReader(output);
            while (!sr.EndOfStream)
            {
                string[] line = sr.ReadLine().Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);

                if (!average.ContainsKey(line[0]))
                    average.Add(line[0], double.Parse(line[1].ToString()));
                else
                    average[line[0]] += double.Parse(line[1].ToString());
            }
            sr.Close();
        }

        public static IList<int> FindTopHeadItems(string training_path, string test_path)
        {
            var training = ItemData.Read(training_path);
            var test = ItemData.Read(test_path);

            IList<int> test_items = (test != null) ? test.AllItems : new int[0];

            ItemPopularity2 recommender = new ItemPopularity2();
            recommender.Feedback = training;
            recommender.Train();

            IList<Tuple<int, float>> prediction = new List<Tuple<int, float>>();
            float num_training = 0f;
            int max_id = recommender.MaxItemID;
            for (int item_id = 1; item_id <= max_id; item_id++)
            {
                float score = recommender.Predict(1, item_id);
                num_training += score;
                prediction.Add(new Tuple<int, float>(item_id, score));
            }
            prediction = prediction.OrderByDescending(x => x.Item2).ToArray();
            

            List<int> top_head_items = new List<int>();
            float top_head = 0f;
            //int num_popular_items = 100;
            for (int i = 0; i < max_id; i++)
            {
                top_head += prediction[i].Item2;
                if ((top_head / num_training) * 100f > 30f) break;
                else
                    top_head_items.Add(prediction[i].Item1);
            }

            return top_head_items;
        }

        public static IList<int> ReadTopHeadItems(StreamReader path)
        {
            List<int> top_head_items = new List<int>();

            while (!path.EndOfStream)
            {
                string[] line = path.ReadLine().Split(new char[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);

                top_head_items.Add(int.Parse(line[0].ToString()));
            }

            return top_head_items;
        }
        //iter_ 추가
        public static float pow(int a)
        {
            float b = 0.1f;
            for (int i = 0; i < a; i++)
                b = b / 10;
            return b;
        }
        public static ItemRecommender SelectPosOnlyFeedbackAlgorithm(Algorithm alg,int ri, int iter_)
        {
            switch (alg)
            {
                case Algorithm.ItemPopularity2:
                    return new ItemPopularity2();

                case Algorithm.BPRMF:
                    return new BPRMF(500, pow(ri), pow(iter_) , pow(iter_) , 0.01f);
          

                case Algorithm.WRMF:
                    return new WRMF(500, 20, pow(ri), iter_);

                default:
                    return new BPRLinear();
            }
        }
    }
}
