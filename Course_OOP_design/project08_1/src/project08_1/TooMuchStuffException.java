package project08_1;

public class TooMuchStuffException extends Exception {

	private int inputNumber;
	
	public TooMuchStuffException(String message/* your code */) {
		/* your code */
		super(message);
	}
	
	public TooMuchStuffException() {
		/* your code */
		super("Too much stuff!");
	}
	
	public TooMuchStuffException(int num/* your code */) {
		/* your code */
		inputNumber = num;
	}
	
	public int getNumber() {
		/* your code */
		return inputNumber;
	}
}
