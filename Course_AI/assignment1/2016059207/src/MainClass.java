/*
 * 학과: 컴퓨터소프트웨어공학부
 * 학번: 2016059207
 * 이름: 박대영
 * 
 * 과제1에 대한 소스코드 작성을 완료하였습니다
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

	public static void makeOutput(int N, String path) { // 이 함수 내에서 과제로 주어진 3개의 알고리즘이 수행됩니다
		PrintWriter outputStream = null;
		
		String str_N = Integer.toString(N);
		String OsFilePath = path.replaceAll(Pattern.quote("\\"), Matcher.quoteReplacement(File.separator));
		//System.out.println(OsFilePath);
		Dfs d = new Dfs(N); // dfs 알고리즘입니다
		long beforeTime1 = System.currentTimeMillis();
		d.start(); // 이 함수를 호출하면 결과에 해당하는 문자열이 Dfs 클래스 내에 생성됩니다
		long afterTime1 = System.currentTimeMillis();
		
		Bfs b = new Bfs(N); // bfs 알고리즘입니다
		long beforeTime2 = System.currentTimeMillis();
		b.start(); // 이 함수를 호출하면 결과에 해당하는 문자열이 bfs 클래스 내에 생성됩니다
		long afterTime2 = System.currentTimeMillis();
		
		Dfs_ID did = new Dfs_ID(N); // dfid 알고리즘입니다
		long beforeTime3 = System.currentTimeMillis();
		did.start(); // 이 함수를 호출하면 결과에 해당하는 문자열이 Dfs_id 클래스 내에 생성됩니다
		long afterTime3 = System.currentTimeMillis();
		
		try {
			outputStream = new PrintWriter(new FileOutputStream(OsFilePath + "\\result" + str_N + ".txt"));
			
			outputStream.print(">DFS(cnt=");
			outputStream.print(d.cnt_generating_node); outputStream.println(')');
			if(d.findAns) {
			outputStream.println(d.make_result()); // make_result 가 resultN.txt에 쓸 문자열을 반환해줍니다
			outputStream.print("Time : ");
			outputStream.println( (afterTime1 - beforeTime1) / 1000.0); // 시간은 '초' 단위로 출력되도록 하였습니다
			outputStream.println();
			}
			else {
				outputStream.println("No solution \n Time : 0.0 \n");
				outputStream.println();
			}
			
			outputStream.print(">BFS(cnt=");
			outputStream.print(b.cnt_generating_node); outputStream.println(')');
			if(d.findAns) {
			outputStream.println(b.make_result()); // make_result 가 resultN.txt에 쓸 문자열을 반환해줍니다
			outputStream.print("Time : ");
			outputStream.println( (afterTime2 - beforeTime2) / 1000.0); // 시간은 '초' 단위로 출력되도록 하였습니다
			outputStream.println();
			}
			else {
				outputStream.println("No solution \n Time : 0.0 \n");
				outputStream.println();
			}
			
			outputStream.print(">DFID(cnt=");
			outputStream.print(did.cnt_generating_node); outputStream.println(')');
			if(did.findAns) {
			outputStream.println(did.make_result()); // make_result 가 resultN.txt에 쓸 문자열을 반환해줍니다
			outputStream.print("Time : ");
			outputStream.println( (afterTime3 - beforeTime3) / 1000.0); // 시간은 '초' 단위로 출력되도록 하였습니다
			outputStream.println();
			}
			else {
				outputStream.println("No solution \n Time : 0.0 \n");
			}
			
		}
		catch (FileNotFoundException e) {
			System.out.println("Problem making file.");
			System.exit(0);
		}
		
		outputStream.close();
	}
}
