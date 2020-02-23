// Copyright (C) 2011, 2012, 2013 Zeno Gantner
// Copyright (C) 2010 Zeno Gantner, Steffen Rendle
//
// This file is part of MyMediaLite.
//
// MyMediaLite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MyMediaLite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MyMediaLite.  If not, see <http://www.gnu.org/licenses/>.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using MyMediaLite.Data;
using MyMediaLite.DataType;
using MyMediaLite.Eval.Measures;
using MyMediaLite.ItemRecommendation;
using System.IO;

/*! \namespace MyMediaLite.Eval
 *  \brief This namespace contains evaluation routines.
 */
namespace MyMediaLite.Eval
{
    /// <summary>Evaluation class for item recommendation</summary>
    public static class Items2
    {
        /// <summary>the evaluation measures for item prediction offered by the class</summary>
        /// <remarks>
        /// The evaluation measures currently are:
        /// <list type="bullet">
        ///   <item><term>AUC</term><description>area under the ROC curve</description></item>
        ///   <item><term>prec@5</term><description>precision at 5</description></item>
        ///   <item><term>prec@10</term><description>precision at 10</description></item>
        ///   <item><term>MAP</term><description>mean average precision</description></item>
        ///   <item><term>recall@5</term><description>recall at 5</description></item>
        ///   <item><term>recall@10</term><description>recall at 10</description></item>
        ///   <item><term>NDCG</term><description>normalizad discounted cumulative gain</description></item>
        ///   <item><term>MRR</term><description>mean reciprocal rank</description></item>
        /// </list>
        /// An item recommender is better than another according to one of those measures its score is higher.
        /// </remarks>
        static public ICollection<string> Measures
        {
            get
            {
                var measures = new HashSet<string>();

                measures.Add("ATOP");

                measures.Add("MRR");

                measures.Add("AUC");

                measures.Add("HLU");

                var positions = new int[] { 5, 10, 15, 20, 25, 30, 50 };
                foreach (int pos in positions)
                    measures.Add("mrr@" + pos);
                foreach (int pos in positions)
                    measures.Add("auc@" + pos);
                foreach (int pos in positions)
                    measures.Add("prec@" + pos);

                foreach (int pos in positions)
                    measures.Add("recall@" + pos);

                foreach (int pos in positions)
                    measures.Add("NDCG@" + pos);

                return measures;
            }
            //         get {
            //	string[] measures = { "AUC", "prec@5", "prec@10", "MAP", "recall@5", "recall@10", "NDCG", "MRR" };
            //	return new HashSet<string>(measures);
            //}
        }

        /// <param name="candidate_items">a list of integers with all candidate items</param>
        /// <param name="candidate_item_mode">the mode used to determine the candidate items</param>
        /// <param name="test">test cases</param>
        /// <param name="training">training data</param>
        static public IList<int> Candidates(
            IList<int> candidate_items,
            CandidateItems candidate_item_mode,
            IPosOnlyFeedback test,
            IPosOnlyFeedback training)
        {
            IList<int> test_items = (test != null) ? test.AllItems : new int[0];
            IList<int> result = null;

            switch (candidate_item_mode)
            {
                // All items setting
                case CandidateItems.UNION:
                    result = test_items.Union(training.AllItems).ToList();
                    break;

                // Long-tail items setting
                case CandidateItems.LONGTAIL:

                    result = test_items.Union(training.AllItems).ToList();
                    if (Program.top_head_items == null)
                    {
                        ItemPopularity2 recommender = new ItemPopularity2();
                        recommender.Feedback = training;
                        recommender.Train();

                        IList<Tuple<int, float>> prediction = new List<Tuple<int, float>>();
                        int max_id = recommender.MaxItemID;
                        for (int item_id = 1; item_id <= max_id; item_id++)
                        {
                            float score = recommender.Predict(1, item_id);
                            prediction.Add(new Tuple<int, float>(item_id, score));
                        }
                        prediction = prediction.OrderByDescending(x => x.Item2).ToArray();

                        int num_popular_items = 100;
                        for (int i = 0; i < num_popular_items; i++)
                            result.Remove(prediction[i].Item1);
                    }
                    else
                    {
                        //int num_popular_items = 100;
                        //int num_popular_items = 1452;
                        //for (int i = 0; i < num_popular_items; i++)
                        foreach (int items in Program.top_head_items)
                            result.Remove(items);
                    }
                                        
                    break;

                case CandidateItems.EXPLICIT:
                    if (candidate_items == null)
                        throw new ArgumentNullException("candidate_items");
                    result = candidate_items.ToArray();
                    break;

                default:
                    throw new ArgumentException("Unknown candidate_item_mode: " + candidate_item_mode.ToString());
            }

            result.Shuffle();
            return result;
        }

        /// <summary>Evaluation for rankings of items</summary>
        /// <remarks>
        /// User-item combinations that appear in both sets are ignored for the test set, and thus in the evaluation,
        /// except the boolean argument repeated_events is set.
        ///
        /// The evaluation measures are listed in the Measures property.
        /// Additionally, 'num_users' and 'num_items' report the number of users that were used to compute the results
        /// and the number of items that were taken into account.
        ///
        /// Literature:
        /// <list type="bullet">
        ///   <item><description>
        ///   C. Manning, P. Raghavan, H. Schütze: Introduction to Information Retrieval, Cambridge University Press, 2008
        ///   </description></item>
        /// </list>
        ///
        /// On multi-core/multi-processor systems, the routine tries to use as many cores as possible,
        /// which should to an almost linear speed-up.
        /// </remarks>
        /// <param name="recommender">item recommender</param>
        /// <param name="test">test cases</param>
        /// <param name="training">training data</param>
        /// <param name="test_users">a list of integers with all test users; if null, use all users in the test cases</param>
        /// <param name="candidate_items">a list of integers with all candidate items</param>
        /// <param name="candidate_item_mode">the mode used to determine the candidate items</param>
        /// <param name="repeated_events">allow repeated events in the evaluation (i.e. items accessed by a user before may be in the recommended list)</param>
        /// <param name="n">length of the item list to evaluate -- if set to -1 (default), use the complete list, otherwise compute evaluation measures on the top n items</param>
        /// <returns>a dictionary containing the evaluation results (default is false)</returns>
        static public Dictionary<string, double> Evaluate(
            this IRecommender recommender,
            IPosOnlyFeedback test,
            IPosOnlyFeedback training,
            IList<int> test_users = null,
            IList<int> candidate_items = null,
            CandidateItems candidate_item_mode = CandidateItems.OVERLAP,
            RepeatedEvents repeated_events = RepeatedEvents.No,
            int n = -1)
        {
            if (test_users == null)
                test_users = test.AllUsers;
            candidate_items = Candidates(candidate_items, candidate_item_mode, test, training);

            var result = new Dictionary<string, double>();
            foreach (string measure in Items2.Measures)
                result[measure] = 0;

            // make sure that the user matrix is completely initialized before entering parallel code
            var training_user_matrix = training.UserMatrix;
            var test_user_matrix = test.UserMatrix;

            int num_users = 0;
            foreach (int user_id in test_users)
            {
                num_users++;
                var correct_items = new HashSet<int>(test_user_matrix[user_id]);
                correct_items.IntersectWith(candidate_items);
                if (correct_items.Count == 0)
                    continue;

                var ignore_items_for_this_user = new HashSet<int>(
                    repeated_events == RepeatedEvents.Yes || training_user_matrix[user_id] == null ? new int[0] : training_user_matrix[user_id]
                );

                ignore_items_for_this_user.IntersectWith(candidate_items);
                int num_candidates_for_this_user = candidate_items.Count - ignore_items_for_this_user.Count;
                if (correct_items.Count == num_candidates_for_this_user)
                    continue;

                var prediction = recommender.Recommend(user_id, candidate_items: candidate_items, n: n, ignore_items: ignore_items_for_this_user);
                var prediction_list = (from t in prediction select t.Item1).ToArray();

                int num_dropped_items = num_candidates_for_this_user - prediction.Count;
                var positions = new int[] { 5, 10, 15, 20, 25, 30, 50 };

                // evaluate Top-N items for each measure 
                var atop = ATOP.Compute(prediction_list, correct_items);
                var mrr = ReciprocalRank.Compute(prediction_list, correct_items);
                var mrr_n = ReciprocalRank.ComputeAt(prediction_list, correct_items, positions);
                var prec = PrecisionAndRecall.PrecisionAt(prediction_list, correct_items, positions);
                var recall = PrecisionAndRecall.RecallAt(prediction_list, correct_items, positions);
                var ndcg = NDCG.NDCGAt(prediction_list, correct_items, positions);
                var auc = AUC.Compute(prediction_list, correct_items, num_dropped_items);
                var hlu = HLU.Compute(prediction_list, correct_items);

                if (training_user_matrix[user_id] == null)
                {
                    result["ATOP"] += 0;
                    result["MRR"] += 0;
                    result["AUC"] += 0;
                    result["HLU"] += 0;
                    foreach (int pos in positions)
                    {
                        result["prec@" + pos] += 0;
                        result["recall@" + pos] += 0;
                        result["NDCG@" + pos] += 0;
                        result["mrr@" + pos] += 0;
                    }
                }
                else
                {
                    result["ATOP"] += atop;
                    result["MRR"] += mrr;
                    result["AUC"] += auc;
                    result["HLU"] += hlu;
                    foreach (int pos in positions)
                    {
                        result["prec@" + pos] += prec[pos];
                        result["recall@" + pos] += recall[pos];
                        result["NDCG@" + pos] += ndcg[pos];
                        result["mrr@" + pos] += mrr_n[pos];
                    }
                }

                //Console.WriteLine(num_users);
                //if (num_users == 100)
                //    break;
            }            

            foreach (string measure in Measures)
                result[measure] /= num_users;
            result["num_users"] = num_users;
            result["num_lists"] = num_users;
            result["num_items"] = candidate_items.Count;

            return result;
        }        
    }
}
