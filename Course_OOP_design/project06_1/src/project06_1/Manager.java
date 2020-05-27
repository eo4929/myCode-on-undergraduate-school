package project06_1;

public class Manager extends Employee {
	int officeNum;
	String team;
	
	public Manager(String name, int employeeNum, int officeNum, String team) {
		/* your code */
		super(name,employeeNum);
		//super.name = name;
		//super.employeeNum = employeeNum;
		this.officeNum = officeNum;
		this.team = team;
	}
	
	public boolean equals(Manager compare) {
		/* your code */
		if(super.equals(compare) == true &&
				this.officeNum == compare.officeNum &&
				this.team.equals(compare.team))
			return true;
		else
			return false;
	}
	
	public String toString() {
		/* your code */
		return ("Name "+this.name+"\n"+"location : "+this.department+","+this.officeNum);
	}
}