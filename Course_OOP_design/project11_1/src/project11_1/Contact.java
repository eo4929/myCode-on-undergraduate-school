package project11_1;

public class Contact implements Comparable/* your code */{
	private String name;
	private String telNum;
	private String email;
	
	public Contact(String name, String telNum, String email) {
		/* your code */
		this.name = name;
		this.telNum = telNum;
		this.email = email;
	}
	
	public Contact(String name) {
		/* your code */
		this.name = name;
		this.telNum = null;
		this.email = null;
	}
	
	public String getTelNum() {	
		/* your code */
		return this.telNum;
		}
	public void setTelNum(String telNum) { 
		/* your code */
		this.telNum = telNum;
		}
	public String getEmail() { 
		/* your code */
		return this.email;
		
		}
	public void setEmail(String email) { 
		/* your code */
		this.email = email;
		}
	public String getName() { 
		/* your code */
		return this.name;
		}
	
	public String toString() {
		return "Name: " + name + "\ttelNum: " + telNum + "\temail: " + email + "\n";
	}
	
	public boolean equals(Object obj) {
		/* your code */
		Contact objContact = (Contact) obj;
		//String objString = (String)obj;
		if(this.name.equals(objContact.getName()))
			return true;
		else
			return false;
	}
	
	public int compareTo(Object obj) {
		if(obj==null) throw new NullPointerException("Object is null");
		else if(this.getClass()!=obj.getClass()) throw new ClassCastException("Object not of the same type");
		else {
			Contact otherManager = (Contact) obj;
			int compare = this.getName().compareTo(otherManager.getName());
			return compare;
		}
	}
}
