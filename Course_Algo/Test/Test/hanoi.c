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

	if (isClockAlreadyCalled == 0)
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

long long hanoi_topdown(int n)
{
	++call_count;

	if (n == 1)
		return 1;

	return hanoi_topdown(n - 1) + 1 + hanoi_topdown(n - 1);
}

#include <stdlib.h>


long long hanoi_topdown2(int n)
{
	int isFirstCall = 0;
	static long long *data = 0;

	call_count++;
	
	if (data == 0)
	{
		int idx;
		isFirstCall = 1;

		if (n == 1) // 실수 주의! 원반 1개일때만 1일만 옮기면 됨
			return 1;
		
		data = malloc(sizeof(long long) * (n + 1));
		data[1] = 1;
		
		for (idx = 2; idx <= n; ++idx)
			data[idx] = 0;
	}

	if (data[n] != 0)
		return data[n];

	data[n] = hanoi_topdown2(n - 1) + 1 + hanoi_topdown2(n - 1);

	if (isFirstCall == 1) // callstack에서 마지막 함수 호출 끝날때 동적할당해제
	{
		long long result = data[n];
		free(data);
		data = 0;

		return result;
	}
	else
		return data[n];
}



long long hanoi_topdown3(int n) //fibo with DP(last) 메모리 절약까지 한 bottom-up버전
{
	int idx;
	long long data;


	data = 1;

	for (idx = 2; idx <= n; ++idx)
	{
		data = data + 1 + data;
	}

	++call_count;

	return data;
}


/*
Variable to customize next run
*/
int(*fib)(int n) = hanoi_topdown2;




int main()
{
	int number;
	long long result;
	double elapsedTime;

	while (1)
	{
		call_count = 0;

		printf("Enter number>");
		if (scanf("%d", &number) == 0 || number < 1)
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

		printf("hanoi(%d) -> %lld\n"
			"Call count: %lld\n"
			"Elapsed time: %.15f seconds\n", number, result, call_count, elapsedTime);
	}

	printf("Done.\n");

	return 0;
}