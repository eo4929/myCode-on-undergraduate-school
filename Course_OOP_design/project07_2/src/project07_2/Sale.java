package project07_2;

public abstract class Sale {

	private String name;
	private double price;
	private double deliverFee;
	
	public Sale() {
		name = "No name yet";
		price = 0;
		deliverFee = 0;
	}
	
	public Sale(String theName, double thePrice) {
		setName(theName);
		setPrice(thePrice);
	}
	
	public Sale(Sale originalObject) {
		if (originalObject == null) {
			System.out.println("Error: null Sale object.");
			System.exit(0);
		}
		name = originalObject.name;
		price = originalObject.price;
	}
	
	public static void announcement() {	System.out.println("This is the Sale class"); }
	
	public void showAdvertisement() {
		announcement();
		System.out.println(toString());
	}
	
	public double getPrice() { return price; }
	
	public void setPrice (double newPrice) {
		if (newPrice >= 0) price = newPrice;
		else {
			System.out.println("Error: Negative price.");
			System.exit(0);
		}
	}
	
	public void setName(String newName) {
		if(newName != null && newName != "") name = newName;
		else {
			System.out.println("Error: Improper name value.");
			System.exit(0);
		}
	}
	
	public String toString() { return (name + " Price and total cost = $" + price); }
	
	public double bill() { return price; }
	
	public String getName() {
		return this.name;
	}
	
	public boolean equalDeals(Sale otherSale) {
		if (otherSale == null) return false;
		else return (name.equals(otherSale.name) && bill() == otherSale.bill());
	}
	
	public boolean lessThan(Sale otherSale) {
		if (otherSale == null) {
			System.out.println("Error: null Sale object");
			System.exit(0);
		}
		return (bill() < otherSale.bill());
	}
	
	public boolean equals(Object otherObject) {
		if(otherObject == null)
			return false;
		else if (getClass() != otherObject.getClass())
			return false;
		else {
			Sale otherSale = (Sale) otherObject;
			return (name.equals(otherSale.name) && (price == otherSale.price));
		}
	}
	
	/* your code */
	public abstract double deliverFee();
	
	
	public boolean equalDeliverFee(Sale otherSale) {
		/* your code */
		if(this.deliverFee() == otherSale.deliverFee())
			return true;
		
		return false;
	}
	
}
