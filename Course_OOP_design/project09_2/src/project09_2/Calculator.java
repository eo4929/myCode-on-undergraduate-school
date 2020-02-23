package project09_2;

public class Calculator {
	private Candidate[] candidateList;
	private int count;
	
	public Calculator() {
		candidateList = new Candidate[10];
		this.count = 0;
	}
	
	public int getNameCnt() {
		return this.count;
	}
	
	public void addName(String name) {
		this.candidateList[count++] = new Candidate(name);
	}
	
	public String toString(int i) {
		return this.candidateList[i].toString();
	}
	
	public String toStringAllList() {
		String str = new String();
		for(int i=0; i<this.count; i++) {
			str += this.candidateList[i].toString();
		}
		return str;
	}
}
