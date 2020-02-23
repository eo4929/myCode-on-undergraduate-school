using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using MyMediaLite.Data;
using MyMediaLite.DataType;
using MyMediaLite.Taxonomy;

namespace MyMediaLite.Correlation
{
    public sealed class AsymmetricMatching : SymmetricCorrelationMatrix, IRatingCorrelationMatrix
    {
        public AsymmetricMatching(int num_entities) : base(num_entities) { }

        public float ComputeCorrelation(IRatings ratings, EntityType entity_type, IList<Tuple<int, float>> entity_ratings, int j)
        {
            throw new NotImplementedException();
        }

        public float ComputeCorrelation(IRatings ratings, EntityType entity_type, int i, int j)
        {
            throw new NotImplementedException();
        }

        public void ComputeCorrelations(IRatings ratings, EntityType entity_type)
        {
            int num_entities = (entity_type == EntityType.USER) ? ratings.MaxUserID + 1 : ratings.MaxItemID + 1;
            Resize(num_entities);

            if (entity_type != EntityType.USER && entity_type != EntityType.ITEM)
                throw new ArgumentException("entity type must be either USER or ITEM, not " + entity_type);

            IList<IList<int>> ratings_by_other_entity = (entity_type == EntityType.USER) ? ratings.ByItem : ratings.ByUser;
            
            var overlaps = new SymmetricMatrix<int>(num_entities);
            var overlaps_rating = new SymmetricMatrix<int>(num_entities);
            int[] count = new int[num_entities];   

            foreach (IList<int> other_entity_ratings in ratings_by_other_entity)
                for (int i = 0; i < other_entity_ratings.Count; i++)
                {
                    var index1 = other_entity_ratings[i];
                    int x = (entity_type == EntityType.USER) ? ratings.Users[index1] : ratings.Items[index1];
                    count[x]++;

                    // update pairwise scalar product and frequency
                    for (int j = i + 1; j < other_entity_ratings.Count; j++)
                    {
                        var index2 = other_entity_ratings[j];
                        int y = (entity_type == EntityType.USER) ? ratings.Users[index2] : ratings.Items[index2];

                        float rating1 = (float)ratings[index1];
                        float rating2 = (float)ratings[index2];

                        // 두 user (혹은 두 item)이 공통으로 평가한 items (혹은 users) 수 
                        overlaps[x, y] += 1;
                        // 두 user (혹은 두 item)이 공통으로 평가한 items (혹은 users) 중 rating이 같은 items (혹은 users) 수
                        if (rating1 == rating2)
                            overlaps_rating[x, y] += 1;
                    }
                }

            // the diagonal of the correlation matrix
            for (int i = 0; i < num_entities; i++)
                this[i, i] = 1;

            for (int i = 0; i < num_entities; i++)
                for (int j = i + 1; j < num_entities; j++)
                {
                    double numerator = overlaps_rating[i, j];

                    double denominator = count[i] + count[j] - overlaps[i, j];

                    if (denominator == 0)
                    {
                        this[i, j] = 0;
                        continue;
                    }                    

                    this[i, j] = (float)(numerator / denominator);
                }
        }
    }
}
