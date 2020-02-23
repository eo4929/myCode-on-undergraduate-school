package project04_2;

public class Cities {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		City city1 = new City("Seoul");
		City city2 = new City("Paris", 133, 251);
		City city3 = new City("Paris", 133, 251);
		City city4 = new City("Raccon City", 30, 250);
		City city5 = new City("Mega City", 310, 170);

		System.out.println("Seoul Information: " + city1.toString());
		System.out.println("Mega City Information: "+ city2.toString());
		System.out.println();
		
		System.out.println("Paris-Paris: " + city2.equals(city3));
		System.out.println("Seoul-Paris: " + city1.equals(city2));
		System.out.println();
		
		System.out.println("Seoul-Pris: " + City.cityDistance(city1, city2));
		System.out.println("Seoul-Racoon City: " + City.cityDistance(city1, city4));
		System.out.println("Paris-Mega City: " + City.cityDistance(city2, city5));
		
	}

}
