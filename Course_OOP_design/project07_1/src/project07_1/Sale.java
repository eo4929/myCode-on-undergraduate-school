package project07_1;

public class Sale {

	private String name;
	private double price;
	
	public Sale() {
		name = "No name yet";
		price = 0;
	}
	
	public Sale(String theName, double thePrice) {
		setName(theName);
		setPrice(thePrice);
	}
	
	public Sale(Sale originalObject) {
		if (originalObject == null) {
			System.out.println("Error: null sale object.");
			System.exit(0);
		}
		name = originalObject.name;
		price = originalObject.price;
	}
	
	public static void announcement() {	System.out.println("This is the Sale class"); }
	
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
		/* your code */
		if(this.getName() == otherSale.getName() &&
				this.bill() == otherSale.bill() )
			return true;
		
		return false;
	}
	
	public boolean lessThan(Sale otherSale) {
		/* your code */
		if(this.bill() < otherSale.bill())
			return true;
		
		return false;
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
}
