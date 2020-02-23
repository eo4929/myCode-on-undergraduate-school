// Gauss-jordan 소거법

#include <stdio.h>
#include <math.h>

//피벗을 구하기 위한 함수
void BubbleSort(double subA[], int n); // 버블 정렬 알고리즘 -> 피벗을 찾기 위함
double printPivot(double subA[]); // 피벗을 구하기 위한 함수
void searchPivotIndex(double pivot,double cpyA[][4]); // 피벗의 행과 열을 구하기 위한 함수
void inspectDiagonal(double pivot,double cpyA[][4]); // 피벗이 대각선에 위치했는지 검사하는 함수
void exchangeLine(double cpyA[][4],int originalLine,int changedLine); // 행 교환을 위한 함수
void makeFromPivotToOne(double pivot,double cpyA[][4]); // pivot을 1로 만드는 함수
void insertToNotPivotLine(double cpyA[][4]); // 후진대입하는 함수
void insertFromcpyAToA(double A[][3], double cpyA[][4]); // 피벗팅 완료한 피벗팅 탐색에서 제거 , A에 변경된 cpyA의 값들을 대입하기 위한 함수

int pivot_column,pivot_line; // pivot의 index
int count;

int main(void)
{
	//초기화 과정
	double A[3][3] = {{3.0,1.0,1.0},{-1.0,1.0,-2.0},{2.0,0.0,-1.0}};
	double X[3] = {0.0,}; // 연립방정식의 해 , 0으로 초기화
	double B[3] = {2.0,7.0,5.0};
	double subA[9] = {0.0,}; // 버블 정렬 알고리즘을 사용하기 위한 배열 , 0으로 초기화
	double cpyA[3][4] = {0.0,}; // Gauss-Jordan 소거법을 적용할 배열 , 0으로 초기화 , 확대행렬
	double pivot=0; //pivot
	
	
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			cpyA[i][j] = A[i][j]; //A값 확대행렬에 대입
		}
	}
	for(int i=0; i<3; i++)
	{
		cpyA[i][3] = B[i]; // B값 확대행렬에 대입
	}

	// Gauss-Jordan 소거법 시작
	for(int i=0; i<3; i++)
{
	count++;
	int k=0;
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			subA[k++] = A[i][j]; // A의 모든 값 subA에 대입
		}
	}
	BubbleSort(subA,sizeof(subA)/sizeof(double)); // 1. pivot 구하기 위한 버블 정렬 알고리즘
	pivot = printPivot(subA); // 2. pivot 구하기
	searchPivotIndex(pivot,cpyA); // 3. pivot의 index 구하기
	inspectDiagonal(pivot,cpyA); // 4. pivot이 대각선에 위치하고 있는지 검사 5. 행 교환
	makeFromPivotToOne(pivot,cpyA); // 5. pivot을 1로 만들기
	insertToNotPivotLine(cpyA); // 6. 후진대입
	insertFromcpyAToA(A,cpyA); // 7. Gauss-Jordan 소거법 후 변경된 행렬 본래 행렬에 대입
}
	//결과
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<4; j++)
		{
			printf("%0.3lf  ", cpyA[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
	for(int i=0; i<3; i++)
	{
		X[i] = cpyA[i][3];
	}

	for(int i=0; i<3; i++)
	{
		printf("X%d = %0.3lf ",i+1,X[i]);
	}
	printf("\n");

	return 0;
}

void BubbleSort(double arr[], int n)
{
	int i, j;
	double temp;

	for(i=0; i<n-1; i++) // pivot을 가장 마지막 배열에 저장하기 위한 반복문
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

double printPivot(double subA[])
{
	return subA[8]; // pivot 반환
}

void searchPivotIndex(double pivot,double cpyA[][4])
{
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			if(pivot == cpyA[i][j])
			{
				pivot_column = j;
				pivot_line = i;
			}
		}
	}
}

void inspectDiagonal(double pivot,double cpyA[][4])
{
	if(count==3)
	{
		return;
	}
	if( (pivot_line == 0 && pivot_column == 0) || (pivot_line == 1 && pivot_column == 1) 
		|| (pivot_line == 2 && pivot_column == 2) ) // pivot 이 대각선이라면
	{
	}
	else // pivot이 대각선이 아니라면
	{
		if(pivot_line==0) // pivot이 0번째 행인 경우
		{
			if(pivot_column==1) // pivot이 1번째 열인 경우
			{
				exchangeLine(cpyA,0,1);
				pivot_line++;
			}
			if(pivot_column==2) // pivot이 2번째 열인 경우
			{
				exchangeLine(cpyA,0,2);
				pivot_line+=2;
			}
		}
		else if(pivot_line==1) // pivot이 1번째 행인 경우
		{
			if(pivot_column==0) // pivot이 0번째 열인 경우
			{
				exchangeLine(cpyA,1,0);
				pivot_line--;
			}
			if(pivot_column==2) // pivot이 2번째 열인 경우
			{
				exchangeLine(cpyA,1,2);
				pivot_line++;
			}
		}
		else if(pivot_line==2) // pivot이 2번째 행인 경우
		{
			if(pivot_column==0) // pivot이 0번째 열인 경우
			{
				exchangeLine(cpyA,2,0);
				pivot_line -=2;
			}
			if(pivot_column==1) // pivot이 1번째 열인 경우
			{
				exchangeLine(cpyA,2,1);
				pivot_line--;
			}
		}
	}
}

void exchangeLine(double cpyA[][4],int originalLine,int changedLine)
{
	double temp[3][4] = {0,};

	for(int i=0; i<4; i++) // 행 교환
	{
		temp[changedLine][i] = cpyA[changedLine][i];
		cpyA[changedLine][i] = cpyA[originalLine][i];
		cpyA[originalLine][i] = temp[changedLine][i];
	}
}

void makeFromPivotToOne(double pivot,double cpyA[][4])
{
	for(int i=0; i<4; i++)
	{
		cpyA[pivot_line][i] /= pivot;
	}
}

void insertToNotPivotLine(double cpyA[][4])
{
	double pivotline[4];
	double changedNotpivotline1[4];
	double changedNotpivotline2[4];

	if(pivot_line == 0)
	{
		for(int i=0; i<4; i++) // 계수행렬에 있는 원소들 얻어오기 
		{
			pivotline[i] = cpyA[pivot_line][i];
			changedNotpivotline1[i] = cpyA[1][i];
			changedNotpivotline2[i] = cpyA[2][i];
		}
		for(int i=0; i<4; i++) // 연산 후 대입
		{
			pivotline[i] *= (-changedNotpivotline1[pivot_column]);
			cpyA[1][i] = pivotline[i] + changedNotpivotline1[i];

			pivotline[i] = cpyA[pivot_line][i];

			pivotline[i] *= (-changedNotpivotline2[pivot_column]);
			cpyA[2][i] = pivotline[i] + changedNotpivotline2[i];
		}
	}
	else if(pivot_line == 1)
	{
		for(int i=0; i<4; i++) // 계수행렬에 있는 원소들 얻어오기 
		{
			pivotline[i] = cpyA[pivot_line][i];
			changedNotpivotline1[i] = cpyA[0][i];
			changedNotpivotline2[i] = cpyA[2][i];
		}
		for(int i=0; i<4; i++) // 연산 후 대입
		{
			pivotline[i] *= (-changedNotpivotline1[pivot_column]);
			cpyA[0][i] = pivotline[i] + changedNotpivotline1[i];

			pivotline[i] = cpyA[pivot_line][i];

			pivotline[i] *= (-changedNotpivotline2[pivot_column]);
			cpyA[2][i] = pivotline[i] + changedNotpivotline2[i];
		}
	}
	else
	{
		for(int i=0; i<4; i++) // 계수행렬에 있는 원소들 얻어오기 
		{
			pivotline[i] = cpyA[pivot_line][i];
			changedNotpivotline1[i] = cpyA[0][i];
			changedNotpivotline2[i] = cpyA[1][i];
		}
		for(int i=0; i<4; i++) // 연산 후 대입
		{
			pivotline[i] *= (-changedNotpivotline1[pivot_column]);
			cpyA[0][i] = pivotline[i] + changedNotpivotline1[i];

			pivotline[i] = cpyA[pivot_line][i];

			pivotline[i] *= (-changedNotpivotline2[pivot_column]);
			cpyA[1][i] = pivotline[i] + changedNotpivotline2[i];
		}
	}
}

void insertFromcpyAToA(double A[][3], double cpyA[][4])
{
	if(count==1)
	{
		if(pivot_line==0) // pivot이 0번째 행인 경우
		{
			for(int i=0; i<3; i++)
			{
				A[1][i] = cpyA[1][i];
				A[2][i] = cpyA[2][i];
			}
		}
		else if(pivot_line==1) // pivot이 1번째 행인 경우
		{
			for(int i=0; i<3; i++)
			{
				A[0][i] = cpyA[0][i];
				A[2][i] = cpyA[2][i];
			}
		}
		else if(pivot_line==2) // pivot이 2번째 행인 경우
		{
			for(int i=0; i<3; i++)
			{
				A[0][i] = cpyA[0][i];
				A[1][i] = cpyA[1][i];
			}
		}

		for(int i=0; i<3; i++)
		{
			A[pivot_line][i] = 0;
		}
	}
	if(count==2)
	{
		//대각선이 1이 아닌 행 찾고, A에 넣어주기
		if(cpyA[0][0] != 1.0)
		{
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					A[i][j] = 0;
				}
			}

			A[0][0] = cpyA[0][0];
		}
		else if(cpyA[1][1] != 1.0)
		{
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					A[i][j] = 0;
				}
			}

			A[1][1] = cpyA[1][1];
		}
		else if(cpyA[2][2] != 1.0)
		{
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<3; j++)
				{
					A[i][j] = 0;
				}
			}

			A[2][2] = cpyA[2][2];
		}
	}
}