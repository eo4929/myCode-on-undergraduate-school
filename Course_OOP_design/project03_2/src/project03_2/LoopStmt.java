package project03_2;

/**
 * Int형 변수 두 개(n, m)를 사용하여, 곱셈한 결과를 Console창에 출력하라.
 * 
 * 요청사항
 * 		- n과 m은 1부터 5까지이다.
 * 		- 출력할 문구는 'n multiplied by m = n*m' 이다. (n, m, n*m에는 각각의 값이 출력되야한다.) 
 */

public class LoopStmt {

	public static void main(String[] args) {
		/* 학생 코드 작성 부분 - 사이에 코드를 작성하시오. */
		int intVal1 , intVal2;

		for(intVal1 = 1; intVal1<=5; intVal1++)
		{
			for(intVal2 = 5; 0<intVal2; intVal2--)
			{
				System.out.println(intVal1 + " multiplied by " + intVal2 + " = " +  intVal1*intVal2);   
			}
		}
		/* 학생 코드 작성 부분 - 사이에 코드를 작성하시오. */
	}

}
