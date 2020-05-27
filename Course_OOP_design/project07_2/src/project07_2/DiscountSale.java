package project07_2;

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
	
	public DiscountSale(DiscountSale originalObject) {
		super(originalObject);
		discount = originalObject.discount;
	}

	public static void announcement() {	System.out.println("This is the Discount class."); }
	
	public double bill() { return (1 - discount/100)*getPrice(); }
	
	public double getDiscount() { return discount; }
	
	public void setDiscount(double newDiscount) {
		if (newDiscount >= 0)
			discount = newDiscount;
		else {
			System.out.println("Error: Negative discount.");
			System.exit(0);
		}
	}
	
	public String toString() { return (getName() + " Price = $" + getPrice() + " Discount = " + discount + "%\n" + " Deliver Fee = $" + deliverFee()); }
	
	public boolean equal(Object obj) {
		if(obj==null)
			return false;
		else if(getClass()!=obj.getClass())
			return false;
		else {
			DiscountSale temp = (DiscountSale) obj;
			return (super.equals(temp) && (discount == temp.discount));
		}
	}
	
	public double deliverFee() {
		/* your code */
		if(this.getDiscount() >= 80)
			return 3;
		
		else if(30 <=this.getDiscount() &&
				this.getDiscount()<= 80)
			return 2;
		
		else
			return 0;
	}
}
