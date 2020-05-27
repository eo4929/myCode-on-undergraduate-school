package project03_3;

public class DateFirstTryDemo {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		/*Write the Code*/
		DateFirstTry date1 = new DateFirstTry();
		date1.day = 3;
		date1.month = "march";
		
		DateFirstTry date2 = new DateFirstTry();;
		date2.makeItNewYears();
		
		System.out.println("date1: " + date1.yellIfNewYear());
		System.out.println("date2: " + date2.yellIfNewYear());

	}

}
