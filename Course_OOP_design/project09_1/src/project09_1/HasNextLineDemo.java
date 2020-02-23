package project09_1;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.util.Scanner;


public class HasNextLineDemo {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner inputStream = null;
		PrintWriter outputStream = null;
		
		try {
			
			/* your code */
			inputStream = new Scanner(new FileInputStream("resource\\BladeRunner.txt"));
			String line = null;
			String line2 = null;
			int i = 0;
			outputStream = new PrintWriter(new FileOutputStream("resource\\NumberedRunner.txt"));
			
			while(inputStream.hasNextLine())
			{
				i++;
				line = inputStream.nextLine();
				line2 = i + ". " + line;
				
				outputStream.println(line2);
			}
			
			/* Input File Location Example: resource\\BladeRunner.txt */
			
		} catch (FileNotFoundException e) {
			System.out.println("Problem opening files.");
			System.exit(0);
		}
		
		/* your code */
		inputStream.close();
		outputStream.close();
	}

}
