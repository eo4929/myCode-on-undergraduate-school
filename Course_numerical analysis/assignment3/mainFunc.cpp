// Gauss-seidel 반복법

#include <stdio.h>
#include <math.h>
#include <Windows.h>

void BubbleSort(double subA[], int n); // 버블 정렬 알고리즘 -> 가장 큰 계수를 찾기 위함
double printselect(double subA[]); // 가장 큰 계수를 구하기 위한 함수
void inspectDiagonal(double select,double cpyA[][5]); // select가 대각선에 위치했는지 검사하는 함수
void exchangeLine(double cpyA[][5],int originalLine,int changedLine); // 행 교환을 위한 함수

int select_line0,select_line1,select_line2,select_line3; // select의 행
int k=0; // 행에서 가장 계수가 큰 미지수를 구하기 위한 반복 변수
double e=0.001; // 허용오차

int main(void)
{
	//초기화 과정
	double A[4][4] = {{12.0,-2.0,-2.0,-3.0},{2.0,7.0,-1.0,1.0},{0.0,1.0,5.0,-2.0},{1.0,2.0,0.0,-8.0}}; // 연립방정식
	double X[4] = {0.0,}; // 연립방정식의 해 , 0으로 초기화
	double P[20][4] = {0.0,}; // 반복되는 식 P
	double B[4] = {5,9,4,-5}; // 상수열
	double subA[4] = {0.0,}; // 버블 정렬 알고리즘을 사용하기 위한 배열 , 0으로 초기화
	double cpyA[4][5] = {0.0,}; // 해에 대한 식을 만들기 위한 배열 , 0으로 초기화
	double select[4]={0,}; //select
	
	
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			cpyA[i][j] = A[i][j]; //A값 대입
		}
	}
	for(int i=0; i<4; i++)
	{
		cpyA[i][4] = B[i]; // B값 대입
	}

	// Jacobi 반복법 시작

for(int i=0; i<4; i++) // 1. 각각의 식에서 가장 계수가 큰 미지수 구하기
{
	for(int j=0; j<4; j++)
	{
			subA[j] = A[k][j]; // A의 모든 값 subA에 대입
	}
	BubbleSort(subA,sizeof(subA)/sizeof(double)); // 1) select 구하기 위한 버블 정렬 알고리즘
	select[k] = printselect(subA); // 2) select 구하기(계수가 가장 큰 미지수 구하기)
	//printf("%lf \n",select[k]);
	k++; // 다음행에서 가장 큰 계수 찾기
}

for(int i=0; i<4; i++) // 2. 각각의 식을 가장 큰 계수로 나누기
{
	for(int j=0 ; j<5; j++)
	{
		cpyA[i][j] /= select[i];
	}
}

for(int i=0; i<4; i++) // 3. select가 대각선에 위치하고 있는지 검사 4. 행 교환
{
	inspectDiagonal(select[i],cpyA); 
}

//5. 반복되는 P 정리
int num=0;
for(num=1; num<20; num++) // 탈출 조건 1
{
	// 바뀐 부분 : 각 P[num][숫자]에서 직전에 완성된 p[num][숫자]들을 바로 사용함!
	P[num][0] = ((-cpyA[0][1])*P[num-1][1]) + ((-cpyA[0][2])*P[num-1][2]) + ((-cpyA[0][3])*P[num-1][3]) + cpyA[0][4];
	P[num][1] = ((-cpyA[1][0])*P[num][0]) + ((-cpyA[1][2])*P[num-1][2]) + ((-cpyA[1][3])*P[num-1][3]) + cpyA[1][4];
	P[num][2] = ((-cpyA[2][0])*P[num][0]) + ((-cpyA[2][1])*P[num][1]) + ((-cpyA[2][3])*P[num-1][3]) + cpyA[2][4];
	P[num][3] = ((-cpyA[3][0])*P[num][0]) + ((-cpyA[3][1])*P[num][1]) + ((-cpyA[3][2])*P[num][2]) + cpyA[3][4];

	printf("P[%d] : ",num-1);
	for(int j=0; j<4; j++)
	{
		printf("%0.3lf\t",P[num-1][j]);
	}
	printf("\n");

	if( (P[num][0] - P[num-1][0]) < e && (P[num][1] - P[num-1][1]) < e && (P[num][2] - P[num-1][2]) < e && (P[num][3] - P[num-1][3]) < e ) // 탈줄 조건 2
		break;
}
	printf("\n");
	printf("결과 : x1=%0.3lf x2=%0.3lf x3=%0.3lf x4=%0.3lf\n",P[num][0],P[num][1],P[num][2],P[num][3]);
	
system("pause");

return 0;
}

void BubbleSort(double arr[], int n)
{
	int i, j;
	double temp;

	for(i=0; i<n-1; i++) // select을 가장 마지막 배열에 저장하기 위한 반복문
	{
		for(j=0; j<(n-i)-1; j++)
		{
			if(fabs(arr[j]) > fabs(arr[j+1]))
			{
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

double printselect(double subA[])
{
	return subA[3]; // select 반환
}

void inspectDiagonal(double select,double cpyA[][5]) // select가 대각선에 위치했는지 검사하는 함수
{
	if( select == cpyA[0][0] || select == cpyA[1][1] || select == cpyA[2][2] || select == cpyA[3][3] )
	{
	}
	else // select이 대각선이 아니라면
	{
		if(select == cpyA[0][1]){exchangeLine(cpyA,1,0);}
		if(select == cpyA[0][2]){exchangeLine(cpyA,2,0);}
		if(select == cpyA[0][3]){exchangeLine(cpyA,3,0);}

		if(select == cpyA[1][0]){exchangeLine(cpyA,0,1);}
		if(select == cpyA[1][2]){exchangeLine(cpyA,2,1);}
		if(select == cpyA[1][3]){exchangeLine(cpyA,3,1);}

		if(select == cpyA[2][0]){exchangeLine(cpyA,0,2);}
		if(select == cpyA[2][1]){exchangeLine(cpyA,1,2);}
		if(select == cpyA[2][3]){exchangeLine(cpyA,3,2);}

		if(select == cpyA[3][0]){exchangeLine(cpyA,0,3);}
		if(select == cpyA[3][1]){exchangeLine(cpyA,1,3);}
		if(select == cpyA[3][2]){exchangeLine(cpyA,2,3);}
	}
}

void exchangeLine(double cpyA[][5],int originalLine,int changedLine) // 행 교환을 위한 함수
{
	double temp[4][5] = {0,};

	for(int i=0; i<5; i++) // 행 교환
	{
		temp[changedLine][i] = cpyA[changedLine][i];
		cpyA[changedLine][i] = cpyA[originalLine][i];
		cpyA[originalLine][i] = temp[changedLine][i];
	}
}