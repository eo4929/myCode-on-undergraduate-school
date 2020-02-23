
// 각 개체를 의미합니다

public class Individual {
	public int fitness; // 개체의 fitness 점수입니다
	int[] curState = null; // 개체의 state(board 판 의미)입니다 => 인덱스가 row이고 그 인덱스 안에 있는 값이 column을 의미합니다
	public Individual(int N) {
		fitness =0;
		curState = new int[N];
	}
}
