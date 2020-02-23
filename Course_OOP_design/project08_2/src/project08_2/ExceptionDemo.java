package project08_2;

import java.util.Scanner;

public class ExceptionDemo {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner keyboard = new Scanner(System.in);
		int inputNum;
		while(true) {
			try {
				inputNum = keyboard.nextInt();
				if(inputNum == 808)
					break;
				exerciseMethod(inputNum);
			} catch (Exception e){
				System.out.println("Exception is caught in main");
			}
		}
		System.out.println("End of loop");
	}
	
	public static void exerciseMethod(int number) throws Exception/* your code */ {
		/* your code */
		NegativeNumberException e = new NegativeNumberException();
		
		try {
		if(number > 0)
		{
			//System.out.println("in finally block");
			throw new Exception();
			//System.out.println("in finally block");
		}
		else if(number <0)
		{
			System.out.println("This number cannot be accepted!!");
			throw new NegativeNumberException("Exception is caught in exerciseMethod");
			//System.out.println("This number cannot be accepted!!");
			//System.out.println("Exception is caught in exerciseMethod");
		}
		else if(number == 0)
		{
			// finally block
			// after finally block
			System.out.println("No Exception");
		}
		}
		catch(NegativeNumberException e2)
		{
			System.out.println(e2.getMessage());
		}
		finally
		{
			System.out.println("in finally block");
		}
		System.out.println("after finally block");
	}
}
