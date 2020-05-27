package project03_4;

public class Employee {

	/* Write the code */
	private String name;
	private int age;
	private String position;
	private int salary;
	private int vacationDays;
	
	public Employee() {}
	
	
	public Employee(String name, int age) {
		/* Write the code */
		this.name = name;
		this.age = age;
		this.position = "Engineer";
		this.salary = 15000;
		this.vacationDays = 20;
	}
	
	public Employee(String name, int age, String position, int salary) {
		/* Write the code */
		this.name = name;
		this.age = age;
		this.position = position;
		this.salary = salary;
		this.vacationDays = 20;
	}
	
	public Employee(String name, int age, String position, int salary, int vacationDays) {
		/* Write the code */
		this.name = name;
		this.age = age;
		this.position = position;
		this.salary = salary;
		this.vacationDays = vacationDays;
	}
	
	public void setSalary(int salary) {
		/* Write the code */
		this.salary = salary;
	}
	
	public void outInfo() {
		/* Write the code */
		System.out.println("name : " + this.name);
		System.out.println("age : " + this.age);
		System.out.println("name : " + this.position);
		System.out.println("name : " + this.salary);
		System.out.println("name : " + this.vacationDays);
	}
	
	public String vacation(int num) {
		/* Write the code */
		if(this.vacationDays >= num)
		{
			this.vacationDays -=num;
			return "Possible";
		}
		else
		{
			return "Impossible";
		}
	}
}
