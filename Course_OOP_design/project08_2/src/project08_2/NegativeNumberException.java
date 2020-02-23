package project08_2;

public class NegativeNumberException extends Exception{
	
	public NegativeNumberException() {
		/* your code */
		super("This number cannot be accepted!!");
	}
	
	public NegativeNumberException(/* your code */String message) {
		/* your code */
		super(message);
	}	
}
