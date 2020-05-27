/*
 * Bfs 알고리즘을 구현하는 클래스입니다
 * 
 * 이 클래스 내에서 만든 변수들과 함수들이 Dfs 클래스에서 만든 변수들, 함수들과 동일한 이름을 가질 떄, 이들은 같은 의미를 지닙니다
 */

import java.util.ArrayList;
import java.util.Arrays;

import java.util.Queue;
import java.util.LinkedList;

public class Bfs {
	static int n = 0; 
	static ArrayList<Integer> answer = null; 
	static int[] rootnode = null; 
	static TreeNode<int[]> root = null; 
	static boolean findAns = false; 
	Queue<TreeNode<int[]>> q = null; // bfs를 구현하기 위해 사용할 큐입니다. 여기에 있는 노드들을 하나씩 꺼내어 goal test를 하고 expanding합니다
	static int cnt_generating_node = 0; 
	
	public Bfs(int N) {
		n = N;
		answer = new ArrayList<>(n+1); 
		for(int i=0; i<n+1; i++) {
			answer.add(0);
		}
		rootnode = new int[N+1];
		Arrays.fill(rootnode, 0);
		root = new TreeNode<int[]>(rootnode);
		q = new LinkedList<TreeNode<int[]>>(); // 큐를 생성합니다
		cnt_generating_node=0;
	}
	
	public void start() {
		q.offer(root); // 아직 queen을 배치하지 않은 state를 큐에 넣습니다.
		bfs(root);
	}
	
	public String make_result() {
		String str = "Location : ";
		for(int i : answer) {
			String s = Integer.toString(i);
			str += (s + " ");
		}
		str += '\n';
		return str;
	}
	
	public void bfs(TreeNode<int[]> node) {
		
		while(true) {
			if(q.isEmpty()==true) { // 무한 루프를 도는데 큐가 비면 답을 찾지 못했음을 의미합니다
				//System.out.println("empty");
				break;
			}
			
			TreeNode<int[]> parent = q.poll(); // goal test와 expanding을 위해 큐에서 노드를 꺼냅니다
			//System.out.print("parent: ");
			//System.out.println(Arrays.toString(parent.data));
			
			if( parent.depth == n && isGoalPath(parent)) { //goal test를 합니다
				findAns = true;
				for(int k=0; k<n; k++) {
					//System.out.println(parent.data[k+1]);
					answer.set(parent.data[k+1],k); // 첫 goal state를 찾으면 answer 변수에 답을 작성하고 리턴합니다
				}
				answer.remove(0);
				return;
			}
			else if (parent.depth !=n) { // 꺼낸 노드(parent)가 마지막 depth에 있는 노드라면 더이상 expanded되지 않도록 합니다
			for(int j=1; j<n+1; j++) {
				int[] childData = new int[n+1];
				System.arraycopy(parent.data, 0, childData, 0, parent.data.length);
				TreeNode<int[]> childNode = parent.addChild(childData);
				childNode.depth = parent.depth+1; // 자식은 부모노드보다 depth 값이 하나 늘어납니다
				childNode.data[childNode.depth] = j;
				
				cnt_generating_node++;
				q.offer(childNode); // 생성한 자식 노드들을 큐에 넣습니다
				//System.out.println(Arrays.toString(childNode.data));
			}
			}
		}
	}
	
	public boolean isGoalPath(TreeNode<int[]> node) {
		int[] col = node.data;
	    
		for(int c = 2; c<n+1; c++) {
	    for (int i = 1; i < c; i++) {
	        
	        if (col[i] == col[c]) {
	            return false;
	        }
	        
	        if (Math.abs(col[i] - col[c]) == Math.abs(i - c)) {
	            return false;
	        }
	    }
		}
	    return true;
	}
}