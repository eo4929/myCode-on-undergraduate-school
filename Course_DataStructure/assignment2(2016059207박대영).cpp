#include <stdio.h>

int num = 0; //횟수 카운팅

void hanoi(int n,char from, char middle,char to)
	{
	        if(n==1)
	       {
	              printf("%d번 원반을 %c에서 %c로 이동\n",n,from,to); //탈출조건
				  num += 1;
	       }
	        else
	       {
	              hanoi(n-1,from,to,middle); //n-1개의원반수를A에서 B로이동
	              printf("%d번 원반을 %c에서 %c로 이동\n",n,from,to); //맨아래원반을A에서 C로이동
				  num += 1;
	              hanoi(n-1,middle,from,to); //n-1개의원반수를B에서 C로이동
	       }
	}

int main()
{
	int n;
	printf("원반의 개수를 입력하시오 : ");
	scanf("%d", &n);

	hanoi(n , 'A', 'B', 'C');

	printf("총이동횟수 : %d\n",num); // 여기서부터 시작하면 된다.
	return 0;
}

