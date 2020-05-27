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
using System.Globalization;
using System.Text;
using System.IO;
using System.Linq;
using MyMediaLite.IO;
using MyMediaLite.Data;


namespace MyMediaLite.RatingPrediction
{
    public class ItemPopularity : IncrementalRatingPredictor
    {
        /// <summary>The number of observed ratings for each item</summary>
        protected int[] item_ratings_count;

        ///
        public override IRatings Ratings
        {
            set
            {
                base.Ratings = value;
            }
        }

        /// <summary>Train the recommender for each item</summary>
        public override void Train()
        {
            int num_entities = ratings.MaxItemID + 1;
            item_ratings_count = new int[num_entities];
            for (int i = 0; i < num_entities; i++)
                item_ratings_count[i] = 0;
            
            int num = 0;
            for (int index = 0; index < ratings.Count; index++)
            {
                if (Ratings[ratings.Users[index], ratings.Items[index]] != 0f)
                {
                    num++;
                    item_ratings_count[ratings.Items[index]]++;
                }
            }
        }

        /// <summary>Predict the popularity score for recommendation</summary>
        /// <param name="user_id">the user id</param>
        /// <param name="item_id">the item id</param>
        /// <returns>the number of observed ratings for a given item</returns>
        public override float Predict(int user_id, int item_id)
        {
            if (item_id > ratings.MaxItemID || user_id > ratings.MaxUserID)
                return -1;

            return item_ratings_count[item_id];
        }
    }
}
