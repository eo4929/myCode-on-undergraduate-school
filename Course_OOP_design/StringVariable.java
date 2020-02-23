package project01_2;


public class StringVariable {

	public static void main(String[] args) {
		// TODO Auto-generated method stub	
		String name = "parkdaeyoung";
		
		String greeting1 = "Hello";
		String greeting2 = "nice to meet you";
		
		String uName = name.toUpperCase();
		
		System.out.println(greeting1 + " " + name + " " + greeting2);
		
		System.out.println("name의 길이 : " + name.length());
		
		System.out.println("name과 uName은  같은가 : " + name.equals(uName));
		
		System.out.println("name과 uName은 대소문자가 같을때 같은가 : " + name.equalsIgnoreCase(uName));
		
		
	}

}
