package project12_1;

public class ChocolateFactory {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		//ChocolateBoiler a = new ChocolateBoiler();
		//ChocolateBoiler b = new ChocolateBoiler();
		ChocolateBoiler a = ChocolateBoiler.getInstance();
		ChocolateBoiler b = ChocolateBoiler.getInstance();
		// Change the above code
		
		a.fill();
		b.fill();
		a.boil();
		b.boil();
		a.drain();
		b.drain();
		
	}

}
