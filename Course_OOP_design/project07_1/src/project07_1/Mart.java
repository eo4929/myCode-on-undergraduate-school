package project07_1;

public class Mart {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		Sale simple = new Sale("floor mat", 10.00);
		DiscountSale discount = new DiscountSale("floor mat", 11.00, 10);
		System.out.println(simple);
		System.out.println(discount);
		
		if(discount.lessThan(simple))
			System.out.println("Discounted item is cheaper.");
		else
			System.out.println("Discounted item is not cheaper");
		
		Sale regularPrice = new Sale("cup holder", 9.90);
		DiscountSale specialPrice = new DiscountSale("cup holder", 11.00, 10);
		System.out.println(regularPrice);
		System.out.println(specialPrice);
		
		if(specialPrice.equalDeals(regularPrice))
			System.out.println("Deals are equal.");
		else
			System.out.println("Deals are not equal.");

	}

}
