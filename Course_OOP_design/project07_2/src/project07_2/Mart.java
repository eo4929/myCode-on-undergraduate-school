package project07_2;

public class Mart {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		DiscountSale discount = new DiscountSale("Bottle", 11.0, 50);
		ExpiredSale expired = new ExpiredSale("Milk", 4.5, 5);
		ExpiredSale expired2 = new ExpiredSale("Yogurt", 5.5, 10);
		System.out.println(discount);
		System.out.println(expired);
		System.out.println(expired2);
		
		System.out.print("Bottle and Milk: ");
		if(discount.equalDeliverFee(expired))
			System.out.println("Same deliver fee");
		else
			System.out.println("Different deliver fee");
		
		System.out.print("Milk and Yogurt: ");
		if(expired.equalDeliverFee(expired2))
			System.out.println("Same deliver fee");
		else
			System.out.println("Different deliver fee");
	}
}
