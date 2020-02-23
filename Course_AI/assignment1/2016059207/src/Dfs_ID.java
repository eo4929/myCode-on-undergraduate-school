/*
 * dfid 알고리즘을 구현하는 클래스입니다
 * 
 * 이 클래스 내에서 만든 변수들과 함수들이 Dfs 클래스에서 만든 변수들, 함수들과 동일한 이름을 가질 떄, 이들은 같은 의미를 지닙니다
 */

import java.util.ArrayList;
import java.util.Arrays;

public class Dfs_ID {
	static int n = 0; // n-queens에서 n을 의미합니다
	static ArrayList<Integer> answer = null; // 과제명세서에 나온대로 각 col에서 queen이 배치된 row위치를 품고 있게 할 변수입니다
	static int[] rootnode = null; // search tree의 root이고 initial state에 해당합니다
	static TreeNode<int[]> root = null;
	static boolean findAns = false; // dfid를 수행하다 첫 solution을 찾으면 dfs_id함수의 recursive call을 멈추게 하기 위해 사용합니다
	static int cnt_generating_node = 0; // generated되는 노드를 counting하기 위해 사용합니다
	
	public Dfs_ID(int N) { // 생성자를 통해 필요한 변수들을 초기화하였습니다
		n = N;
		answer = new ArrayList<>(n+1); 
		for(int i=0; i<n+1; i++) {
			answer.add(0);
		}
		rootnode = new int[N+1];
		Arrays.fill(rootnode, 0);
		root = new TreeNode<int[]>(rootnode);
	}
	
	public boolean findSolution() { // limit값을 한개씩 늘리면서 search를 할 때 해당 limit 내에서 답을 찾았는지 아닌지 확인하는 용도의 함수입니다 
		int sum = 0;
		for(int i: answer) {
			sum += i;
		}
		if (sum == 0) {
			return false;
		}
		else return true;
	}
	
	public void start() { // 주어진 문제의 답을 찾기 위해 MainClass에서 호출해야 할 함수입니다 
		
		for(int limit=1; limit<=n; limit++) {
			dfs_id(0,root,limit); // 이전에 구현한 dfs의 마지막 인자에 cutoff를 위한 limit가 전달되도록 코드를 추가하여 구현하였습니다.
			if(findSolution()) break;
		}
		
		int sum = 0; // solution이 없는 경우를 체크하기 위한 변수입니다. 사실 구현과 크게 관련있지는 않습니다.
		for(int i: answer) {
			sum += i;
		}
		if(sum == 0) {
			//System.out.println("no solution");
			return;
		}
		
		//for(int i : answer) { // 잘 출력되는지 cmd창에 테스트하기 위한 부분
		//	System.out.print(i);
		//	System.out.print(" ");
		//}
		//System.out.println();
	}
	
	public String make_result() { // 결과를 resultN.txt에 쓰기 위해 구현한 함수입니다. dfid 수행 후 얻게 된 답을 반환해주는 함수입니다.
		String str = "Location : ";
		for(int i : answer) {
			String s = Integer.toString(i);
			str += (s + " ");
		}
		str += '\n';
		return str;
	}
	
	public void dfs_id(int row , TreeNode<int[]> node, int limit) { // 실제 dfid 알고리즘에 해당하는 함수입니다. recursive version으로 구현하였습니다.
		if(limit == 0 && row != n) { // cutoff를 위해 작성한 코드입니다
			return;
		}
		
		if (findAns == true) {
			return;
		}
		else {
			if (row == n && isGoalPath(node)) { // 현재 state가 답이라면 답을 찾았다는 표시를 하고 answer 변수에 답을 적습니다
				findAns = true;
				
				for(int i=0; i<n; i++) {
					//System.out.println(node.data[i+1]);
					answer.set(node.data[i+1],i);
				}
				answer.remove(0);
				return;
			}
			else if ( row == n && !isGoalPath(node) ) { // 모든 queen을 다 배치하였는데 답이 아니라면 backtracking하여 다른 branch로 이동합니다.
				return;
			}
			else { // expanding을 하는 부분입니다
				for (int i = 1; i <= n; i++) {
					int[] childData = new int[n+1];
					System.arraycopy(node.data, 0, childData, 0, node.data.length); // 자식노드: 1) 일단 부모의 state를 deep copy한 뒤,
					TreeNode<int[]> childNode = node.addChild(childData); // 이 state 정보를 내 노드에 포함시키고 부모와 연결합니다.
					childNode.data[row+1] = i; // 2) 다음 row에 queen을 배치하는 action 또한 포함시킵니다.
					
					cnt_generating_node++; // generated되는 노드들의 갯수를 셉니다.
					//System.out.println(Arrays.toString(childNode.data));
					dfs_id(row + 1, childNode,limit-1); // limit 값까지만 깊이 탐색을 하도록 다음 레벨로 내려갈때마다 limit 값을 하나씩 줄여주고 
					//generating되는 트리는 해당 함수의 첫 if문에 의해 cutoff될 수 있습니다.
				}
			}
	    
		}
	}
	
	public boolean isGoalPath(TreeNode<int[]> node) { // 인자로 주어진 노드가 goal state인지 확인하는 함수입니다
	    // node.data의 index가 row 이고 그 칸에 들어가는 값이 col입니다. 
	    // ex) col[1] = 1 => 1행 1열, col[2] = 3 => 2행 3열
		int[] col = node.data;
	    
		for(int c = 2; c<n+1; c++) {
	    for (int i = 1; i < c; i++) {
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