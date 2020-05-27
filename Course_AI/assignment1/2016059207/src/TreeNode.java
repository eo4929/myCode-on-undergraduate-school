
/*
 * queen을 배치할 때마다 즉, 각각 action에 대한 정보를 반영한 state를 저장할 트리 자료구조입니다.
 * search tree의 각 Node(state를 포함)는 해당 자료구조로 이루어집니다
 */

import java.util.List;
import java.util.LinkedList;
import java.util.Iterator;

public class TreeNode<T> implements Iterable<TreeNode<T>>{
    public T data; // 현재 queen들의 배치를 표현할 state입니다. T는 int[]로 맵핑되고 index가 row 이고 그 칸에 들어가는 값이 col입니다. 
    public TreeNode<T> parent;
    public List<TreeNode<T>> children;
    
    public int depth; // 각 노드의 depth 정보를 의미합니다(예를 들어, root는 0, root의 자식들은 1). Bfs클래스에서 사용합니다.
 
    public TreeNode(T data) {
        this.data = data;
        this.children = new LinkedList<TreeNode<T>>();
        this.depth = 0;
    }
 
    public TreeNode<T> addChild(T child) { // 제 자식들을 저에게 연결시키기 위해 구현한 함수입니다
        TreeNode<T> childNode = new TreeNode<T>(child);
        childNode.parent = this;
        this.children.add(childNode);
        return childNode;
    }
 
    public Iterator<TreeNode<T>> iterator() {
        // TODO Auto-generated method stub
        return null;
    }
}	