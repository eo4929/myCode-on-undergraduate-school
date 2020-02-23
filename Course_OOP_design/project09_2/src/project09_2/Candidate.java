package project09_2;

public class Candidate {
	private String name;
	private int voteCnt;
	
	public Candidate(String name) {
		this.name = name;
		this.voteCnt = (int)(Math.random()*100) + 1;
	}
	
	public void addVote() {
		this.voteCnt++;
	}
	
	public String getName() {
		return this.name;
	}
	public int getVoteCnt() {
		return this.voteCnt;
	}
	
	public String toString() {
		return "Name: " + this.name + "\r\nVotes: " + this.voteCnt + "\r\n";
	}
}
