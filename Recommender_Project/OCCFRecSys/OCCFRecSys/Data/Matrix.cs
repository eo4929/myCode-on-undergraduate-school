using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace OCCFRecSys
{
    public class Matrix
    {
        string[] line;
        public int num_row;
        public int num_column;
        public Dictionary<int, Dictionary<int, float>> matrix;

        public Matrix(StreamReader sr)
        {
            num_row = int.MinValue;
            num_column = int.MinValue;
            matrix = new Dictionary<int, Dictionary<int, float>>();

            Init(sr);
        }

        /// <summary>
        /// Argument로 들어온 파일을 이용하여 matrix 생성
        /// </summary>
        /// <param name="sr"></param>
        public void Init(StreamReader sr)
        {
            while (sr.EndOfStream == false)
            {
                line = sr.ReadLine().Split(new char[] { '\t' }, StringSplitOptions.RemoveEmptyEntries);
                int user_id = int.Parse(line[0].ToString()) - 1;
                int item_id = int.Parse(line[1].ToString()) - 1;
                //int value = int.Parse(line[2].ToString());
                int value = 0;

                if (num_row < user_id)
                    num_row = user_id;
                if (num_column < item_id)
                    num_column = item_id;

                if (!matrix.ContainsKey(user_id))
                {
                    Dictionary<int, float> items = new Dictionary<int, float>();
                    items.Add(item_id, value);
                    matrix.Add(user_id, items);
                }
                else if (!matrix[user_id].ContainsKey(item_id))
                    matrix[user_id].Add(item_id, value);
            }
            sr.Close();

            num_row++;
            num_column++;
        }

        /// <summary>
        /// 해당 row가 갖는 column 수 반환
        /// </summary>
        /// <param name="row_index"></param>
        /// <returns></returns>
        public int GetNumOfColumns(int row_index)
        {
            if (matrix.ContainsKey(row_index))
                return matrix[row_index].Count();
            else
                return 0;
        }

        /// <summary>
        /// 해당 column이 갖는 row 수 반환
        /// </summary>
        /// <param name="column_index"></param>
        /// <returns></returns>
        public int GetNumOfRows(int column_index)
        {
            int num = 0;
            foreach (int row_index in matrix.Keys)
                if (matrix[row_index].ContainsKey(column_index))
                    num++;
            return num;
        }
    }
}
