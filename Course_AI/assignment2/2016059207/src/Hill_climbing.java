
/*
 * 과제로 주어진 Hill climbing 알고리즘을 구현한 클래스입니다
 */

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class Hill_climbing {
	static int n = 0; // n-queens에서 n을 의미합니다
	static ArrayList<Integer> answer = null; // 과제명세서에 나온대로 각 col에서 queen이 배치된 row위치를 품고 있게 할 변수입니다
	static int[] curState = null; // 현재 state입니다. 각 인덱스가 row 이고 각 칸에 들어가는 값이 col을 의미합니다
	static int[] Successor = null; // 현재 state와 objective function의 값을 비교할 successor입니다
	static int[] localOptimum = null; // Hill climbing 알고리즘이 한번 돌 때 구하게 될 local optimum입니다. 이 값이 실제 솔루션인지 체크되어야 합니다
	static Random random = new Random();
	static boolean findAns = false;
	static int cnt_restart = 0; // 몇 번 restart되고 실제 솔루션을 구하는지 확인하기 위한 변수입니다
	
	public Hill_climbing(int N) { // 생성자 입니다
		n = N;
		answer = new ArrayList<>(n); 
		for(int i=0; i<n; i++) {
			answer.add(0);
		}
		curState = new int[N];
		Successor = new int[N];
		localOptimum = new int[N];
	}
	
	public void restart() { // local optimum이 정답이 아닐때 이 함수를 호출하여 restart합니다
		for(int i=0; i<n; i++) {
			curState[i] = random.nextInt(n); // 퀸의 배치를 다시 합니다
		}
	}
	
	public String make_result() { // 정답 솔루션을 문자열로 출력해주는 함수입니다.
		String str = "Location : ";
		for(int i : answer) {
			String s = Integer.toString(i);
			str += (s + " ");
		}
		str += '\n';
		return str;
	}
	
	public void start() {
		
		for(int i=0; i<n; i++) {
			curState[i] = random.nextInt(n); // initial state 초기화 부분입니다. 명세서에 나온대로 랜덤하게 값을 넣습니다.
		}
		
		if(n==2 || n==3) { // n이 2이거나 3일때는 어차피 답이 없으니 iteration의 횟수를 제한합니다(10번)
			for(int i=0; i<10; i++) {
				hillClimbing(1);
				if(isPerfectSolution(localOptimum)) {
					findAns = true;
					for(int j=0; j<n; j++) {
						answer.set(localOptimum[j],j);
					}
					return;
				}
				else {
					restart();
					cnt_restart++;
				}
			}
		}
		else {
			while(true) { // 답이 존재하는 문제라면 perfect 솔루션을 찾을때까지 반복합니다
			hillClimbing(1); // 이 함수가 반환되면 localOptimum이 구해지는데
			if(isPerfectSolution(localOptimum)) { // 그 localOptimum이 실제 정답인지 확인합니다
				findAns = true;
				for(int j=0; j<n; j++) {
					answer.set(localOptimum[j],j); // 정답이 맞다면 과제 명세서에서 요구한대로 출력하기 위해 각 col에 row값이 들어가도록 합니다
				}
				return;
			}
			else {
				restart(); // 정답이 아니면 restart합니다
				cnt_restart++;
			}
			}
		}
		
		//System.out.println(Arrays.toString(curState));
	}
	
	public void hillClimbing(int row) { //  Hill climbing 알고리즘을 구현하는 함수입니다
		if(row == n) {
			localOptimum = curState;
			return;
		}
		System.arraycopy(curState,0,Successor,0,curState.length);
		Successor[row] = random.nextInt(n); // successor는 현재 state에서 row 변수에 해당하는 row의 값을 랜덤하게 바꿉니다(action). 즉, 한 행에 있는 퀸을 기존 열에서 다른 열로 이동시킵니다.
		int val_current = computeTheNumOfAttack(curState,row); // 나의 objective 함수의 값을 계산
		int val_Succesor = computeTheNumOfAttack(Successor,row); // successor의 objective 함수의 값을 계산
		
		if(val_current<=val_Succesor) { // 낮은 게 좋은 값이고 내가 더 좋으면
			hillClimbing(row+1); // 나를 선택해서 계속 진행합니다
		}
		else { // successor가 더 좋으면 이 state가 current state가 됩니다
			curState = Successor;
			hillClimbing(row+1);
		}
	}
	
	public int computeTheNumOfAttack(int[] state, int row) { // objective function이고 output값이 적어야 좋은 값입니다
		int num_attack = 0; // 퀸 끼리 공격을 많이 할수록 증가하는 값입니다
		
		for (int i = 0; i < row; i++) {
	        // 같은 행, 열
	        if (state[i] == state[row]) {
	        	num_attack+=1;
	        }
	        // 대각선
	        if (Math.abs(state[i] - state[row]) == Math.abs(i - row)) {
	        	num_attack+=1;
	        }
	    }
		
		return num_attack; // 실제 정답이라면 0을 반환할 것입니다
	}
	
	public boolean isPerfectSolution(int[] state) { // hill climbing으로 구한 local optimum이 실제 정답인지 확인합니다
	    // col[1] = 1행 *열
	    // col[1] = 1 => 1행 1열, col[2] = 3 => 2행 3열
		int[] col = state;
	    
		for(int c = 1; c<n; c++) {
	    for (int i = 0; i < c; i++) {
	        // 같은 행, 열에 대해 체크합니다
	        if (col[i] == col[c]) {
	            return false;
	        }
	        // 대각선에 대해 체크합니다
	        if (Math.abs(col[i] - col[c]) == Math.abs(i - c)) {
	            return false;
	        }
	    }
		}
	    return true;
	}
}
