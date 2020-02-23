using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MyMediaLite.Eval.Measures
{
    public static class HLU
    {
        /// <summary>Compute the half-life utility (HLU) of a list of ranked items</summary>
		/// <remarks>
		/// See http://dl.acm.org/citation.cfm?id=963772
		/// </remarks>
		/// <param name="ranked_items">a list of ranked item IDs, the highest-ranking item first</param>
		/// <param name="relevant_items">a collection of positive/correct item IDs</param>
		/// <returns>the HLU for the given data</returns>
        public static double Compute(IList<int> ranked_items, ICollection<int> relevant_items)
        {
            int beta = 5; // halif-life parameter
            double hlu = 0d; // expected utility of the ranked list for users
            double max_hlu = 0d; // maximally achievable utility if all true positive items are at the top of the ranked list

            if (ranked_items.Count == 0)
                return 0d;

            int rank = 1;
            foreach (int item in ranked_items)
            {
                if (relevant_items.Contains(item))
                    hlu += (1 / Math.Pow(2, (rank - 1) * (beta - 1))); // numerator
                max_hlu += (1 / Math.Pow(2, (rank - 1) * (beta - 1))); // denominator
                rank++;
            }

            double lasthlu = 100d * (hlu / max_hlu);

            return lasthlu;            
        }   
    }
}
