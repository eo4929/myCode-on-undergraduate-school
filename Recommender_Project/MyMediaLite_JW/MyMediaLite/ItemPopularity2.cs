using System.Collections.Generic;
using System.Globalization;
using System.IO;
using MyMediaLite.IO;
using MyMediaLite.Data;

namespace MyMediaLite.ItemRecommendation
{
    public class ItemPopularity2 : IncrementalItemRecommender
    {
        /// <summary>The number of observed ratings for each item</summary>
        protected int[] item_ratings_count;

        ///
        public override IPosOnlyFeedback Feedback
        {
            set
            {
                base.Feedback = value;
            }
        }

        /// <summary>Train the recommender for each item</summary>
        public override void Train()
        {
            int num_entities = Feedback.MaxItemID + 1;
            item_ratings_count = new int[num_entities];
            for (int i = 0; i < num_entities; i++)
                item_ratings_count[i] = 0;

            for (int index = 0; index < Feedback.Count; index++)
                item_ratings_count[Feedback.Items[index]]++;
        }

        /// <summary>Predict the popularity score for recommendation</summary>
        /// <param name="user_id">the user id</param>
        /// <param name="item_id">the item id</param>
        /// <returns>the number of observed ratings for a given item</returns>
        public override float Predict(int user_id, int item_id)
        {
            if (item_id > Feedback.MaxItemID || user_id > Feedback.MaxUserID)
                return -1;

            return item_ratings_count[item_id];
        }
    }
}
