package project07_1;

public class DiscountSale extends Sale {

	private double discount;
	
	public DiscountSale() {
		super();
		discount = 0;
	}
	
	public DiscountSale(String theName, double thePrice, double theDiscount) {
		super(theName, thePrice);
		setDiscount(theDiscount);
	}
	
	public DiscountSale (DiscountSale originalObject) {
		super(originalObject);
		discount = originalObject.discount;
	}
	
	public static void announcement() {	System.out.println("This is the Discount class."); }
	
	public double bill() {
		/* your code */
		return super.bill() * ( 100 - this.discount) /100 ;
	}
	
	public double getDiscount() { return discount; }
	
	public void setDiscount(double newDiscount) {
		if (newDiscount >= 0)
			discount = newDiscount;
		else {
			System.out.println("Error: Negative discount.");
			System.exit(0);
		}
	}
	
	public String toString() { return (getName() + " Price = $" + getPrice() + " Discount = " + discount + "%\n" + " Total cost = $" + bill()); }
	
	public boolean equal(Object obj) {
		/* your code */
		if(obj == null)
			return false;
	
		if(getClass() != obj.getClass() )
			return false;
		
		DiscountSale s = (DiscountSale)obj;
		
		if( this.getName() ==  s.getName() &&
				this.bill() == s.bill() &&
				this.getDiscount() == s.getDiscount())
			return true;
		
		return false;
	}
}
