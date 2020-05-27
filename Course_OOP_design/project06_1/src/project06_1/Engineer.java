package project06_1;

public class Engineer extends Employee {
	String workZone;
	String project;
	
	public Engineer(String name, int employeeNum, String workZone, String project) {
		/* your code */
		super(name,employeeNum);
		this.workZone = workZone;
		this.project = project;
	}
	
	public boolean equals(Engineer compare) {
		/* your code */
		if(this.name.equals(compare.name) == true &&
				this.employeeNum == compare.employeeNum &&
				this.project == compare.project &&
				this.workZone == compare.workZone)
		{
			return true;
		}
		
		return false;
	}
	
	public String toString() {
		/* your code */
		return ("Name "+this.name+"\n"+"Emp# : "+this.employeeNum+"\n"+"location :"+this.department+","+this.workZone);
	}
}