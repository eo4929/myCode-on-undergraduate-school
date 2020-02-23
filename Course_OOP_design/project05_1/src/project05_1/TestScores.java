package project05_1;

import java.util.Scanner;

public class TestScores {

	public static final int MAX_NUMBER_SCORES = 10;

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		double[] scores = new double[MAX_NUMBER_SCORES];
		int counting = 0;
		
		System.out.println("This program reads test scores and shows");
		System.out.println("how much each differs from the average.");
		System.out.println("Enter test scores:");
		
		
		counting = fillArray(scores);
		showDifference(scores, counting);
		
	}

	public static int fillArray(double[] scores)
	{
		System.out.println("Mark the end of the list with a negative number.");	
		/* your code */
		Scanner Keyboard = new Scanner(System.in);
		int i;
		
		for(i=0; i<10; i++)
		{
			double v = Keyboard.nextDouble();
			if(v < 0)	
				break;
			else
			{
				scores[i] = v;
			}
		}
		
		return i;
	}
	
	public static void showDifference(double[] scores, int counting)
	{
		double average = computeAverage(scores,counting);
		
		System.out.println("Average of the scores = " + average);
		System.out.println("The scores are: ");
		
		/* your code */
		for(int i=0; i<counting; i++)
		{
			double val = scores[i]-average;
			System.out.println(scores[i]+" differs from average by "+ val);
		}
	}
	
	public static double computeAverage(double[] scores, int counting)
	{	
		/* your code */
		double all=0;
		
		for(int i =0; i<counting; i++)
		{
			all += scores[i]; 
		}
		
		return all/counting;
	}
}
