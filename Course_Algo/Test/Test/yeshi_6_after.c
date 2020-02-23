#include <stdio.h>
#include <stdlib.h>


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


/*
	Fix log(Written in Korean. Sorry.):
		> 변수 data를 함수 안에 숨겨 놓았어요.
		> '첫 호출'때, 함수 내용물 앞머리에서 메모리를 잡고 뒷마무리할 때 해제할 수 있도록 코드를 약간 추가해 두었어요.
			- 더 이상 main()에서 data = 0; 해 줄 필요가 없어짐
			- 수업시간에는 까먹음 ㄷㄷ 미안해요
		> 첫 호출때 n이 1 또는 2더라도 정상 동작하도록 분기 하나를 더 달아 두었어요.
*/

long long fib_topdown2(int n)
{
	int isFirstCall = 0;
	static long long *data = 0;

	++call_count;

	if ( data == 0 )
	{
		int idx;

		isFirstCall = 1;

		if ( n < 3 )
			return 1;

		data = malloc(sizeof(long long) * (n + 1));
		data[1] = 1;
		data[2] = 1;

		for ( idx = 3; idx <= n; ++idx )
			data[idx] = 0;
	}

	if ( data[n] != 0 )
		return data[n];

	data[n] = fib_topdown2(n - 1) + fib_topdown2(n - 2);

	if ( isFirstCall == 1 )
	{
		long long result = data[n];
		free(data);
		data = 0;

		return result;
	}
	else // 
		return data[n];
}


long long fib_topdown3(int n)
{
	int idx;
	int idx_previous;
	long long data[2];

	data[1] = 1;
	data[0] = 1;

	idx_previous = 1;
	// data[idx_previous]: k - 1
	// data[1 - idx_previous]: k - 2 --> k

	for ( idx = 3; idx <= n; ++idx )
	{
		data[1 - idx_previous] = 
			data[1 - idx_previous]
			+ data[idx_previous];

		idx_previous = 1 - idx_previous;
	}

	++call_count;

	return data[idx_previous];
}


/*
	Variable to customize next run
*/
int(*fib)(int n) = fib_topdown2;




int main()
{
	int number;
	long long result;
	double elapsedTime;

	while ( 1 )
	{
		call_count = 0;
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