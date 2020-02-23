package project09_2;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class Election {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Calculator calculator = new Calculator();
		
		try {
			Scanner scanner = new Scanner(new FileInputStream("resource\\candidate.txt"));
			
			/* your code */
			String candidateName = null;
			while(scanner.hasNextLine())
			{
				candidateName = scanner.nextLine();
				
				calculator.addName(candidateName);
				
				//System.out.println(candidateName);
			}
			
			scanner.close();
		} catch (FileNotFoundException e) {
			System.err.println("File not found or already be opened");
			System.exit(0);
		} catch (NoSuchElementException e) {
			System.err.println("Out of Boundary");
			System.exit(0);
		}
		
		try {
			File out = new File("resource\\election.txt");
			/* your code */
			FileOutputStream outFileStream = new FileOutputStream(out);
			PrintWriter outStream = new PrintWriter(outFileStream);
			
			//System.out.println(calculator.toStringAllList());
			
			outStream.println(calculator.toStringAllList());
			
			//outFileStream.close();
			outStream.close();
			
		} catch (IOException e) {
			System.err.println("IOException");
			System.exit(0);
		}
	}

}
