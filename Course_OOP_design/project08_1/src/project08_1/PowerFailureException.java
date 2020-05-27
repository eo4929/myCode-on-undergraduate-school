package project08_1;

public class PowerFailureException extends Exception {

	public PowerFailureException() {
		/* your code */
		super("Power Failure");
	}
	
	public PowerFailureException(String error/* your code */) {
		/* your code */
		super(error);
	}

}
