using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class OCCFMatrix
    {
        int num_user;
        int num_item;

        public void SetTrainMatrix(StreamReader train_file, string weighting_schemes, string file_name) //, int cold1, 
        {
            // training file을 이용하여 matrix 생성 후, OCCF의 weighting scheme로 missing value를 채운 새로운 training set 생성  
            Matrix train_matrix = new Matrix(train_file);
            //num_user = 71567;
            //num_item = 10681;
            num_user = train_matrix.num_row;
            num_item = train_matrix.num_column;

            // User 수가 item 수보다 많으면 user-oriented, 반대면 item-oritented
            Dictionary<int, double> weights = new Dictionary<int, double>();
            if (weighting_schemes == "Distribution-Oriented")
                if (num_user >= num_item)
                    weighting_schemes = "User-Oriented";
                else
                    weighting_schemes = "Item-Oriented";
            weights = ComputeWeights(train_matrix, weighting_schemes, weights);
            Console.WriteLine("Compute weights: Complete");

            // 할당된 weight을 포함하는 training matrix 출력
            StreamWriter new_train_file = new StreamWriter(file_name);
            //StreamWriter new_train_file = new StreamWriter("raw_input\\Density\\u" + fold + "_d" + density + "_" + weighting_schemes + ".base");
            //double positive = 1;
            //double default_value = 0.0000001d;
            #region metadata
            new_train_file.WriteLine("%%MatrixMarket matrix coordinate real general");
            new_train_file.WriteLine(num_user + " " + num_item + " " + (uint)(num_user * num_item));
            #endregion
            for (int i = 0; i < num_item; i++)
            {
                //Console.WriteLine(i);
                for (int j = 0; j < num_user; j++)
                {
                    double weight;
                    // item-oriented인 경우에는 weight이 item별로 할당되어 있고, 나머지는 user별로 할당되어 있음
                    if (weighting_schemes == "Item-Oriented")
                        weight = weights[i];
                    else
                        weight = weights[j];

                    // Weight 할당 (기존에 이미 갖고 있는 아이템에 대해서는 그대로 1 할당)
                    // index가 1에서 시작하도록 해야함
                    if (train_matrix.matrix.ContainsKey(j))
                    {
                        if (train_matrix.matrix[j].ContainsKey(i))
                        {
                            new_train_file.WriteLine((j + 1) + " " + (i + 1) + " 1 1");
                            continue;
                        }
                    }
                    new_train_file.WriteLine((j + 1) + " " + (i + 1) + " " + weight + " 0");
                }
            }
            new_train_file.Flush();
            new_train_file.Close();
        }

        public void SetTestMatrix(StreamReader test_file, string file_name) //
        {
            Matrix test_matrix = new Matrix(test_file);
            //num_user = 71567;
            //num_item = 10681;
            //num_user = train_matrix.num_row;
            //num_item = train_matrix.num_column;

            //StreamWriter new_test_file = new StreamWriter("u" + fold + "_OCCF.test");
            StreamWriter new_test_file = new StreamWriter(file_name);
            #region metadata
            new_test_file.WriteLine("%%MatrixMarket matrix coordinate real general");
            new_test_file.WriteLine("% Generated 28-Aug-2011");
            new_test_file.WriteLine(num_user + " " + num_item + " " + (uint)(num_user * num_item));
            #endregion
            for (int i = 0; i < num_item; i++)
                for (int j = 0; j < num_user; j++)
                    new_test_file.WriteLine((j + 1) + " " + (i + 1) + " 1");
            new_test_file.Flush();
            new_test_file.Close();
        }

        // option에 따라 user_oritented weight, item_oriented weight 혹은 uniform weight 계산
        public Dictionary<int, double> ComputeWeights(Matrix matrix, string weighting_schemes, Dictionary<int, double> weights)
        {
            double max = double.MinValue;
            double min = 0d;
            int num = 0;

            if (weighting_schemes == "Item-Oriented")
            {
                num = num_item;                
                min = num_user;
            }
            else
            {
                num = num_user;
                min = num_item;
            }
            double[] weight = new double[num];

            for (int i = 0; i < num; i++)
            {
                // uniform(0): 모든 wieght을 0.5로 동일하게 할당
                // user_oriented(1): 많은 아이템을 갖는 사용자일 수록 wieght은 올라감
                // item_oriented(2): 많은 사용자가 갖는 아이템일 수록 weight는 내려감
                switch (weighting_schemes)
                {
                    case "Uniform":
                        weight[i] = 0.5d;
                        break;
                    case "User-Oriented":
                        weight[i] = (double)matrix.GetNumOfColumns(i);
                        break;
                    case "Item-Oriented":
                        weight[i] = (double)num_user - (double)matrix.GetNumOfRows(i);
                        break;
                    default:
                        Console.WriteLine("Weighting schemes error");
                        break;
                }

                // normalization을 위해 체크
                // user_oriented: 가장 많은(혹은 적은) 아이템을 갖는 사용자의 아이템 수
                // item_oriented: 가장 많은(혹은 적은) 사용자가 갖는 아이템의 사용자 수
                if (max < weight[i])
                    max = weight[i];
                if (min > weight[i])
                    min = weight[i];
            }

            // normalization
            for (int i = 0; i < num; i++)
            {
                if (weighting_schemes != "Uniform")
                    weight[i] = (weight[i] - min) / (max - min);
                weights.Add(i, weight[i]);
            }

            return weights;
        }
    }
}
