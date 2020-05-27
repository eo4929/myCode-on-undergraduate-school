// Copyright (C) 2010, 2011, 2012 Zeno Gantner
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
// You should have received a copy of the GNU General Public License
// along with MyMediaLite.  If not, see <http://www.gnu.org/licenses/>.
using System;
using System.Collections.Generic;
using System.IO;

namespace MyMediaLite.Data
{
	/// <summary>K-fold cross-validation split for item prediction from implicit feedback</summary>
	/// <remarks>
	/// Items with less than k events associated are ignored for testing and always assigned to the training set.
	///
	/// The dataset must not be modified after the split - this would lead to undefined behavior.
	/// </remarks>
	public class PosOnlyFeedbackCrossValidationSplit<T> : ISplit<IPosOnlyFeedback> where T : IPosOnlyFeedback, new()
    {
		///
		public uint NumberOfFolds { get; private set; }

		///
		public IList<IPosOnlyFeedback> Train { get; private set; }

		///
		public IList<IPosOnlyFeedback> Test { get; private set; }

		/// <summary>Create a k-fold split of positive-only item prediction data</summary>
		/// <remarks>See the class description for details.</remarks>
		/// <param name="feedback">the dataset</param>
		/// <param name="num_folds">the number of folds</param>
		public PosOnlyFeedbackCrossValidationSplit(IPosOnlyFeedback feedback, uint num_folds)
		{
			if (num_folds < 2)
				throw new ArgumentException("num_folds must be at least 2.");

			NumberOfFolds = num_folds;
			Train = new IPosOnlyFeedback[num_folds];
			Test  = new IPosOnlyFeedback[num_folds];
			for (int f = 0; f < num_folds; f++)
			{
				Train[f] = new T();
				Test[f]  = new T();
			}

			// assign events to folds
			int pos = 0;
			foreach (int item_id in feedback.AllItems)
			{
				var item_indices = feedback.ByItem[item_id];

				if (item_indices.Count < num_folds)
				{
					foreach (int index in item_indices)
						for (int f = 0; f < num_folds; f++)
							Train[f].Add(feedback.Users[index], feedback.Items[index]);
				}
				else
				{
					item_indices.Shuffle();

					foreach (int index in item_indices)
					{
						int user_id = feedback.Users[index];
						for (int f = 0; f < num_folds; f++)
							if (pos % num_folds == f)
								Test[f].Add(user_id, item_id);
							else
								Train[f].Add(user_id, item_id);
						pos++;
					}
				}
			}
		}

        /// <summary>Create a k-fold split of positive-only item prediction data</summary>
        /// <remarks>See the class description for details.</remarks>
        /// <param name="ratings">the dataset</param>
        /// <param name="num_folds">the number of folds</param>
        public PosOnlyFeedbackCrossValidationSplit(IRatings ratings, uint num_folds)
        {
            if (num_folds < 2)
                throw new ArgumentException("num_folds must be at least 2.");

            NumberOfFolds = num_folds;
            // randomize
            IList<int> random_indices = ratings.RandomIndex;

            // create index lists
            List<int>[] train_indices = new List<int>[num_folds];
            List<int>[] test_indices = new List<int>[num_folds];

            for (int i = 0; i < num_folds; i++)
            {
                train_indices[i] = new List<int>();
                test_indices[i] = new List<int>();
            }

            // assign indices to folds
            foreach (int i in random_indices)
            {
                for (int j = 0; j < num_folds; j++)
                {
                    if (j == i % num_folds)
                    {
                        if (ratings[ratings.Users[i], ratings.Items[i]] == 5)
                        {
                            test_indices[j].Add(i);
                        }
                    }
                    else
                    {
                        train_indices[j].Add(i);
                    }
                }
            }

            Train = new IPosOnlyFeedback[num_folds];
            Test = new IPosOnlyFeedback[num_folds];
            for (int f = 0; f < num_folds; f++)
            {
                Train[f] = new T();
                Test[f] = new T();
            }

            // create split data structures
            for (int i = 0; i < num_folds; i++)
            {
                //StreamWriter sw1 = new StreamWriter("train" + i + ".txt");
                //StreamWriter sw2 = new StreamWriter("test" + i + ".txt");
                foreach (int index in random_indices)
                {
                    if (train_indices[i].Contains(index))
                    {
                        Train[i].Add(ratings.Users[index], ratings.Items[index]);
                        //sw1.WriteLine(ratings.Users[index] + "\t" + ratings.Items[index] + "\t" + ratings[ratings.Users[index], ratings.Items[index]]);
                    }
                    else if (test_indices[i].Contains(index))
                    {
                        Test[i].Add(ratings.Users[index], ratings.Items[index]);
                        //sw2.WriteLine(ratings.Users[index] + "\t" + ratings.Items[index] + "\t" + ratings[ratings.Users[index], ratings.Items[index]]);
                    }
                }
                //sw1.Close();
                //sw2.Close();
            }
        }

        /// <summary>Create a k-fold split of positive-only item prediction data for cold-start experiment</summary>
        /// <param name="ratings">the dataset</param>
        /// <param name="num_folds">the number of folds</param>
        /// <param name="num_training">the number of training users</param>
        /// <param name="train_given">the number of items rated by testing users</param>
        /// <param name="test_given">the number of items that will be predicted for each testing user</param>
        /// <param name="cold_user">if we will perform cold-user experiments(i.e. UserKNN, SVD), assign true (cold-item experimetns(i.e. ItemKNN) -> false)</param>
        public PosOnlyFeedbackCrossValidationSplit(IRatings ratings, uint num_folds, uint num_training, uint train_given, uint test_given, bool cold_user)
        {
            if (num_folds < 2)
                throw new ArgumentOutOfRangeException("num_folds must be at least 2.");

            NumberOfFolds = num_folds;

            // the number of testing users
            int num_testing = 200;

            int num_max = 0;
            // the number of all users(or items)
            if (cold_user == true)
                num_max = (int)num_training + num_testing;
            else
                num_max = ratings.MaxItemID;

            List<int>[] train_ids = new List<int>[num_folds];
            Dictionary<int, int>[] test_ids = new Dictionary<int, int>[num_folds];

            // create lists of training and testing users' ids
            for (int i = 0; i < num_folds; i++)
            {
                train_ids[i] = new List<int>();
                test_ids[i] = new Dictionary<int, int>();
            }

            System.Random r = new System.Random(1);

            for (int i = 0; i < num_folds; i++)
            {
                while (train_ids[i].Count < num_training)
                {
                    int id = r.Next(1, num_max);
                    if (train_ids[i].Contains(id) == false)
                        train_ids[i].Add(id);
                }
                for (int id = 0; id < num_max; id++)
                    if (train_ids[i].Contains(id) == false && test_ids[i].Count < num_testing)
                        test_ids[i].Add(id, 0);
            }

            // create index lists
            List<int>[] train_indices = new List<int>[num_folds];
            List<int>[] test_indices = new List<int>[num_folds];

            for (int i = 0; i < num_folds; i++)
            {
                train_indices[i] = new List<int>();
                test_indices[i] = new List<int>();
            }

            // assign indices to folds
            for (int i = 0; i < ratings.Count; i++)
            {
                int target_id = 0;

                if (cold_user == true)
                    target_id = ratings.Users[i];
                else
                    target_id = ratings.Items[i];

                for (int j = 0; j < num_folds; j++)
                {
                    if (train_ids[j].Contains(target_id))
                        train_indices[j].Add(i);
                    else if (test_ids[j].ContainsKey(target_id))
                    {
                        if (test_ids[j][target_id] < train_given)
                        {
                            train_indices[j].Add(i);
                            test_ids[j][target_id]++;
                        }
                        else if (test_ids[j][target_id] < train_given + test_given)
                        {
                            if (ratings[ratings.Users[i], ratings.Items[i]] == 5)
                                test_indices[j].Add(i);
                            test_ids[j][target_id]++;
                        }
                    }
                }
            }

            Train = new IPosOnlyFeedback[num_folds];
            Test = new IPosOnlyFeedback[num_folds];
            for (int f = 0; f < num_folds; f++)
            {
                Train[f] = new T();
                Test[f] = new T();
            }

            // create split data structures
            for (int i = 0; i < num_folds; i++)
                for (int index = 0; index < ratings.Count; index++)
                {
                    if (train_indices[i].Contains(index))
                        Train[i].Add(ratings.Users[index], ratings.Items[index]);
                    else if (test_indices[i].Contains(index))
                        Test[i].Add(ratings.Users[index], ratings.Items[index]);
                }
        }
    }
}
