package project05_2;

import java.util.Scanner;

public class EnumValuesDemo {

	/* your code */
	enum WorkDay {MONDAY,TUESDAY,WEDNESDAY,THURSDAY,FRIDAY};
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		/* your code */
		Scanner keyboard = new Scanner(System.in);
		WorkDay[] worktime = WorkDay.values();
		int eachtime = 0;
		int alltime = 0;
		
		for(int i=0; i<worktime.length; i++)
		{
			System.out.println("Enter hour worked for : " + worktime[i]);
			eachtime = keyboard.nextInt();
			if(eachtime <0)
			{
				break;
			}
			alltime += eachtime;
		}
		
		System.out.println("Total hours work = "+alltime);
		
	}
}
