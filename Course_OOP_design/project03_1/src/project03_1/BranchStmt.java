package project03_1;
import java.util.Scanner;
/**
 * Int 값을 키보드를 통해 입력 받은 후, 입력 받은 값이 어느 범위에 속하는지 판하는 프로그램을 구현하라.
 * 
 * 요청사항
 * 		- Scanner를 사용하여 키보드로부터 Int형 변수 n의 값을 입력 받을 것.
 * 		- if 문을 사용하여 입력 받은 n의 값을 아래의 조건대로 구별하여 Console창에 출력할 것.
 * 		- n값 구별 조건
 * 			* n이 0 미만일 경우, 'n is less than 0' 문구를 출력.
 * 			* n이 0 보다 크고 100 미만일 경우, 'n is greater than or equal to 0 and less than 100' 문구를 출력.
 * 			* n이 100보다 클 경우, 'n is greater than or equal to 100' 문구를 출력.
 */

public class BranchStmt {

	public static void main(String[] args) {		
		System.out.print("n = ? ");
		
		/* 학생 코드 작성 부분 - 사이에 코드를 작성하시오. */
		Scanner keyboard = new Scanner(System.in);
		
		int n = keyboard.nextInt();
		
		if(n < 0)
		{
			System.out.println("n is less than 0");
		}
		else if(0 <= n && n < 100)
		{
			System.out.println("n is greater than or equal to 0 and less than 100");
		}
		else
		{
			System.out.println("n is greater than or equal to 100");
		}
		}
		/* 학생 코드 작성 부분 - 사이에 코드를 작성하시오. */
	}

