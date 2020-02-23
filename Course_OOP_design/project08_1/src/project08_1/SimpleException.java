package project08_1;

import java.util.Scanner;

public class SimpleException {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
	while(true) {
		try {
			Scanner keyboard = new Scanner(System.in);
			System.out.println("Enter the number 0 - 9");
			System.out.println("If you enter a negative number, PowerFailureException will occur");
			System.out.println("If you enter a positive number other than 0 - 9, TooMuchStuffException will occur");
			int inputNum = keyboard.nextInt();
			
			if (inputNum < 0)
				throw new PowerFailureException();
			else if (inputNum > 9)
				throw new TooMuchStuffException(inputNum);
			else
				System.out.println("No exception has been occrued");
		} catch (PowerFailureException e) {
			System.out.println(e.getMessage());
		} catch (TooMuchStuffException e) {
			System.out.println(e.getNumber() + " occurs TooMuchStuffException");
		}
		System.out.println("End of try-catch statement");
		System.out.println();
	}

	}

}
