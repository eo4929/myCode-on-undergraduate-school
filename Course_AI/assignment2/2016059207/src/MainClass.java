/*
 * 학과: 컴퓨터소프트웨어공학부
 * 학번: 2016059207
 * 이름: 박대영
 * 
 * 과제2에 대한 소스코드 작성을 완료하였습니다
 */


import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.nio.file.Paths;
import java.io.FileNotFoundException;
import java.io.File;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MainClass {
	
	public static void main(String[] args) {
		int N = Integer.parseInt(args[0]);
		String path = args[1];
		
		makeOutput(N,path); // resultN.txt을 만들기 위한 함수입니다
		//System.out.println(N);
		//System.out.println(path);
	}

	public static void makeOutput(int N, String path) { // 이 함수 내에서 과제로 주어진 알고리즘이 수행됩니다
		PrintWriter outputStream = null;
		
		String str_N = Integer.toString(N);
		String OsFilePath = path.replaceAll(Pattern.quote("\\"), Matcher.quoteReplacement(File.separator));
		//System.out.println(OsFilePath);
		
		Hill_climbing hill = new Hill_climbing(N); // Hill climbing 알고리즘입니다
		long beforeTime = System.currentTimeMillis();
		hill.start(); // 이 함수를 호출하면 결과에 해당하는 문자열이 Hill_climbing 클래스 내에 생성됩니다
		long afterTime = System.currentTimeMillis();
		
		try {
			outputStream = new PrintWriter(new FileOutputStream(OsFilePath + "\\result" + str_N + ".txt"));
			
			outputStream.print(">Hill Climbing(cnt=");
			outputStream.print(hill.cnt_restart); outputStream.println(')');
			if(hill.findAns) {
			outputStream.println(hill.make_result()); // make_result함수가 resultN.txt에 쓸 문자열을 반환해줍니다
			outputStream.print("Total Elapsed Time : ");
			outputStream.println( (afterTime - beforeTime) / 1000.0); // 시간은 '초' 단위로 출력되도록 하였습니다
			outputStream.println();
			}
			else {
				outputStream.println("No solution \n Time : 0.0 \n");
				outputStream.println();
			}
			
			
		}
		catch (FileNotFoundException e) {
			System.out.println("Problem making file.");
			System.exit(0);
		}
		
		outputStream.close();
	}
}
