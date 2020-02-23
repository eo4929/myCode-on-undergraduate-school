package project13_2;

public class StarBuzz {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		Beverage beverage = new Espresso();
		System.out.println(beverage.getDescription() + " $" + beverage.cost()); 	// Espresso
		
		Beverage beverage2 = new DarkRoast();
		beverage2 = new Mocha(beverage2);
		beverage2 = new Mocha(beverage2);
		beverage2 = new Whip(beverage2);
		System.out.println(beverage2.getDescription() + " $" + beverage2.cost()); 	// Dark roast double mocha with whip
		
		Beverage beverage3 = new Decaf();
		beverage3 = new Soy(beverage3);
		System.out.println(beverage3.getDescription() + " $" + beverage3.cost());	// Decaffeine with steamed milk
		
		Beverage beverage4 = new HouseBlend();
		System.out.println(beverage4.getDescription() + " $" + beverage4.cost());	// House blend
		
		Beverage beverage5 = new HyperDark();
		beverage5 = new Cinnamon(beverage5);
		System.out.println(beverage5.getDescription() + " $" + beverage5.cost());	// Hyper dark roast with cinnamon
	}

}
