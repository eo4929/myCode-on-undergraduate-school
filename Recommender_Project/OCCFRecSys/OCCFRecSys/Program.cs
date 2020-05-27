using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Diagnostics;

namespace OCCFRecSys
{
    class Program
    {
        public static string file_path;
        public static StreamReader sr;
        public static StreamWriter sw;
        public static HashSet<int> longtails;
        public static int num_user;

        static void Main(string[] args)
        {           
            string[] candidate_items = { "All_items" }; // ,  "All_items", "Longtail_items"
            string[] algorithms = { "RWR" }; //"RWR", "BP", "OCCF"

            //file_path = "D:\\Downloads\\DataFile\\Movielens\\";
            //num_user = 943;
            //for (int step = 1; step < 2; step += 2)
            //    foreach (string algo in algorithms)
            //        foreach (string candidate_item in candidate_items)
            //            Experiments(algo, candidate_item, step);

            //file_path = "D:\\Downloads\\DataFile\\Watcha\\";
            //num_user = 1391;
            //for (int step = 3; step < 4; step += 2)
            //    foreach (string algo in algorithms)
            //        foreach (string candidate_item in candidate_items)
            //            Experiments(algo, candidate_item, step);

            //file_path = "D:\\Downloads\\DataFile\\Ciao\\";
            //num_user = 996;
            //for (int step = 7; step < 8; step += 2)
            //    foreach (string algo in algorithms)
            //        foreach (string candidate_item in candidate_items)
            //            Experiments(algo, candidate_item, step);

            //file_path = "D:\\Downloads\\DataFile\\CiteULike\\";
            //num_user = 5551;
            //for (int step = 3; step < 4; step += 2)
            //    foreach (string algo in algorithms)
            //        foreach (string candidate_item in candidate_items)
            //            Experiments(algo, candidate_item, step);
        }
        
        public static void Experiments(string algo, string candidate_item, int CASE)
        {
            
            int num_iter = 5;
            int num_recommend = 50;
            double propagation_alpha = 0.0001d;
            double[] crits = { 0.9 }; 

            switch (CASE)
            {
                case 1: // unint 적용 전
                    if (algo == "SignedBP") break;
                    for (int fold = 1; fold <= 5; fold++)
                    {
                        if (candidate_item == "Longtail_items")
                        {
                            longtails = new HashSet<int>();
                            sr = new StreamReader(file_path + "raw\\longtail\\u" + fold + "_longtail_items.txt");
                            while (!sr.EndOfStream)
                            {
                                int item_id = int.Parse(sr.ReadLine()) + num_user;
                                longtails.Add(item_id);
                            }
                            sr.Close();
                        }

                        StreamReader training = new StreamReader(file_path + "raw\\basic\\u" + fold + "\\u" + fold + ".base");
                        StreamReader test = new StreamReader(file_path + "raw\\basic\\u" + fold + "\\u" + fold + ".test");
                        StreamWriter result = new StreamWriter(file_path + "results\\" + algo + "\\unary\\u" + fold + "_" + algo + "_rankresult(" + candidate_item + ").results");

                        if (algo == "RWR")
                        {
                            var recsys = new RWR_Recommender(candidate_item, num_recommend, fold, training, test);
                            recsys.Recommend();
                            recsys.PrintResults(result);
                        }
                        else if (algo == "BP")
                        {
                            var recsys = new BP_Recommender(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test);
                            recsys.Recommend();
                            recsys.PrintResults(result);
                        }
                        
                        result.Close();
                    }
                    break;

                case 2: // unint 적용: 적용 %에 따른 효과 실험
                    foreach (double crit in crits)
                    {
                        for (int fold = 1; fold <= 5; fold++)
                        {
                            if (candidate_item == "Longtail_items")
                            {
                                longtails = new HashSet<int>();
                                sr = new StreamReader(file_path + "raw\\longtail\\u" + fold + "_longtail_items.txt");
                                while (!sr.EndOfStream)
                                {
                                    int item_id = int.Parse(sr.ReadLine().ToString()) + num_user;
                                    longtails.Add(item_id);
                                }
                                sr.Close();
                            }
                                             
                            StreamReader training = new StreamReader(file_path + "unint\\basic\\u" + fold + "\\u" + fold + "_" + crit + ".base");
                            StreamReader test = new StreamReader(file_path + "raw\\basic\\u" + fold + "\\u" + fold + ".test");
                            StreamWriter result = new StreamWriter(file_path + "results\\" + algo + "\\unint\\origin\\u" + fold + "_" + algo + "_rankresult_" + crit + "(" + candidate_item + ").results");

                            if (algo == "RWR")
                            {
                                var recsys = new BalanceRWR_Recommender(candidate_item, num_recommend, fold, 0, training, test);
                                recsys.Recommend();
                                recsys.PrintResults(result);
                            }
                            else if (algo == "BP")
                            {
                                var recsys = new BalanceBP_Recommender(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test);
                                recsys.Recommend();
                                recsys.PrintResults(result);
                            }
                            else if (algo == "SignedBP")
                            {
                                var recsys = new SignedBP_Recommender(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test);
                                recsys.Recommend();
                                recsys.PrintResults(result);
                            }

                            result.Close();
                        }
                    }
                    break;

                case 3:  // balance unint 적용
                    for (int fold = 1; fold <= 1; fold++)
                    {
                        if (candidate_item == "Longtail_items")
                        {
                            longtails = new HashSet<int>();
                            sr = new StreamReader(file_path + "raw\\longtail\\u" + fold + "_longtail_items.txt");
                            while (!sr.EndOfStream)
                            {
                                int item_id = int.Parse(sr.ReadLine().ToString()) + num_user;
                                longtails.Add(item_id);
                            }
                            sr.Close();
                        }                       

                        StreamReader training = new StreamReader(file_path + "unint\\basic\\u" + fold + "\\u" + fold + "_balance.base");
                        StreamReader test = new StreamReader(file_path + "raw\\basic\\u" + fold + "\\u" + fold + ".test");                        
                        StreamWriter result = new StreamWriter(file_path + "results\\" + algo + "\\unint\\origin\\u" + fold + "_" + algo + "_rankresult_balance_" + propagation_alpha + "(" + candidate_item + ").results");
                        if (algo == "RWR")
                        {
                            var recsys = new BalanceRWR_Recommender(candidate_item, num_recommend, fold, 0, training, test);
                            recsys.Recommend();
                            recsys.PrintResults(result);
                        }
                        else if (algo == "BP")
                        {
                            var recsys = new BalanceBP_Recommender(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test);
                            recsys.Recommend();
                            recsys.PrintResults(result);
                        }
                        else if (algo == "SignedBP")
                        {
                            var recsys = new SignedBP_Recommender(candidate_item, num_iter, num_recommend, propagation_alpha, fold, training, test);
                            recsys.Recommend();
                            recsys.PrintResults(result);
                        }

                        result.Close();
                    }
                    break;

                default:
                    Console.WriteLine("Default case");
                    break;
            }
        }        
    }

    public class Pairs
    {
        public Pairs(int user_id, int item_id,double score)
        {
            this.user_id = user_id;
            this.item_id = item_id;
            this.score = score;
        }

        public int user_id;
        public int item_id;
        public double score;

        public static int CompareScore(Pairs a, Pairs b)
        {
            int result = a.score.CompareTo(b.score);

            return result;
        }
    }



}