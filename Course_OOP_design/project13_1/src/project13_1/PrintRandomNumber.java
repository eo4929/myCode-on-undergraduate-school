package project13_1;

public class PrintRandomNumber {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		NumberGenerator a = new RandomNumberGenerator();
		DigitObserver b = new DigitObserver(a);
		GraphObserver c = new GraphObserver(a);
		
		a.execute();
	}

}
