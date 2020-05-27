// Copyright (C) 2011, 2012 Zeno Gantner
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

/*! \namespace MyMediaLite.Eval.Measures
*   \brief This namespace contains different evaluation measures.
*/
namespace MyMediaLite.Eval.Measures
{
    /// <summary>Area under the ROC curve (AUC) of a list of ranked items</summary>
    /// <remarks>
    /// See http://recsyswiki.com/wiki/Area_Under_the_ROC_Curve
    /// </remarks>
    public static class ATOP
    {
        /// <summary>Compute the area under the ROC curve (AUC) of a list of ranked items</summary>
        /// <remarks>
        /// See http://recsyswiki.com/wiki/Area_Under_the_ROC_Curve
        /// </remarks>
        /// <param name="ranked_items">a list of ranked item IDs, the highest-ranking item first</param>
        /// <param name="relevant_items">a collection of positive/correct item IDs</param>
        /// <param name="num_dropped_items">the number of relevant items that were not ranked (considered to be ranked below all ranked_items)</param>
        /// <returns>the AUC for the given data</returns>
        public static double Compute(IList<int> ranked_items, ICollection<int> relevant_items)
        {
            Dictionary<int, int> ranking = new Dictionary<int, int>();
            int max_rank = ranked_items.Count();
            int rank = max_rank;
            foreach (int id in ranked_items)
            {
                ranking.Add(id, rank);
                rank--;
            }

            var relevant_items_in_list = relevant_items.Intersect(ranked_items);
            double ATOP = 0.0d;

            if (relevant_items_in_list.Count() != 0)
            {
                foreach (int id in relevant_items_in_list)
                    ATOP += ((double)(ranking[id] - 1) / (double)(max_rank - 1));
                ATOP = ATOP / (double)relevant_items_in_list.Count();
            }           

            return ATOP;
        }
    }
}
