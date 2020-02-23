package project06_1;

public class Employee {
	String name;
	int employeeNum;
	String department;
	
	public Employee(String name, int employeeNum) {
		/* your code */
		this.name = name;
		this.employeeNum = employeeNum;
		this.department = "No Dept";
		
	}
	
	public String getDepartment() { 
		/* your code */
		return this.department;
	}
	public void setDepartment(String dept) { 
		/* your code */
		this.department = dept;
	}
	
	public boolean equals(Employee compare) {
		/* your code */
		if(this.name.equals(compare.name) == true &&
				this.employeeNum == compare.employeeNum)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public String toString() { 
		/* your code */
		return ("Name "+this.name+"\n"+"Emp# : "+this.employeeNum);
	}
}
