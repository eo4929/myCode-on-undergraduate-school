#include <stdio.h>

// for ordinary VS environments ( x86 + Windows )
#ifdef WIN32
#include <Windows.h>
LARGE_INTEGER startTime, endTime;
LARGE_INTEGER freq;
int isClockAlreadyCalled;

LARGE_INTEGER clock()
{
	LARGE_INTEGER now;

	if ( isClockAlreadyCalled == 0 )
	{
		isClockAlreadyCalled = 1;
		QueryPerformanceFrequency(&freq);
	}

	QueryPerformanceCounter(&now);

	return now;
}

// for Mac, Linux, or Windows x64
#else

#include <time.h>
clock_t startTime, endTime;

#endif









long long call_count = 0;

long long fib_topdown(int n)
{
	++call_count;

	if ( n == 1 || n == 2 )
		return 1;

	return fib_topdown(n - 1) + fib_topdown(n - 2);
}

#include <stdlib.h>

long long *data; // 메타데이터 담기 위한 실제 공간 malloc으로 할당하자


long long fib_topdown2(int n) //fibo with DP
{
	int idx;

	if (data == 0)
	{
		data = malloc(sizeof(long long) * (n + 1)); // 1번째 칸부터 잡기 위해 n에 +1함
		data[1] = 1; // fibo(1) 은 1
		data[2] = 1; // fibo(2) 은 1
	}
		
	for (idx = 3; idx <= n; ++idx)
		data[idx] = 0;

	if (data[n] != 0) //만약 0이면 이 fibo[n]는 처음 구하는 것이다
		return data[n];

	++call_count;

	//if (n == 1 || n == 2) 필요 없어짐
	//	return 1;

	data[n] = fib_topdown2(n - 1) + fib_topdown2(n - 2);

	return data[n];
}

long long fib_topdown3(int n) //fibo with DP(bottom-up)
{
	int idx;

	if (data == 0)
	{
		data = malloc(sizeof(long long) * (n + 1)); // 1번째 칸부터 잡기 위해 n에 +1함
		data[1] = 1; // fibo(1) 은 1
		data[2] = 1; // fibo(2) 은 1
	}

	for (idx = 3; idx <= n; ++idx)
		data[idx] = data[idx-2] + data[idx-1];

	++call_count;

	return data[n];
}

long long fib_topdown4(int n) //fibo with DP(last)
{
	int idx;
	int idx_previous;
	long long data[2];

	data[1] = 1;
	data[0] = 1;

	idx_previous = 1; // 0과 1 어느것으로 잡든 상관없음
	//data[idx_previous] : k-1
	//data[1-idx_previous] : k-2 -> k

	for (idx = 3; idx <= n; ++idx)
	{
		data[1 - idx_previous] = data[1 - idx_previous] + data[idx_previous];

		idx_previous = 1 - idx_previous;
	}

	++call_count;

	return data[idx_previous];
}


/*
	Variable to customize next run
*/
int(*fib)(int n) = fib_topdown3;




int main()
{
	int number;
	long long result;
	double elapsedTime;

	while ( 1 )
	{
		call_count = 0;
		data = 0;

		printf("Enter number>");
		if ( scanf("%d", &number) == 0 || number < 1 )
			break;

		startTime = clock();

		result = fib(number);

		endTime = clock();

#ifdef WIN32
		elapsedTime = endTime.QuadPart - startTime.QuadPart;
		elapsedTime /= freq.QuadPart;
#else
		elapsedTime = endTime;
		elapsedTime -= startTime;
		elapsedTime /= CLOCKS_PER_SEC;
#endif

		printf("fib(%d) -> %lld\n"
			   "Call count: %lld\n"
			   "Elapsed time: %.15f seconds\n", number, result, call_count, elapsedTime);
	}

	printf("Done.\n");

	return 0;
}