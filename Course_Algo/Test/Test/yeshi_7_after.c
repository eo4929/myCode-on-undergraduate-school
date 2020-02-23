/*
	주:
		- 가격표를 임의로 정하고 싶을 때 바꿔 볼 코드들을 make_prices() 함수 정의 안에 몰아 담아 두었어요.
			> 50번째 줄 정도에 있어요
			> 주의: n 값 자체는 main()에서 설정하고 있음!

		- 마지막 출력 타이밍에 웬만한 정보들을 다 보여주도록 꾸며 두었어요.
*/

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

// 첫 구현때 사용할 기본 가격표(길이가 1이면 1번째 칸에 담긴 것을 씀)
int default_prices[] = { 0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };

// r_0 ~ r_n까지를 담을 예정
int *r;
int *idxs;

// 새 가격표를 만들고 싶을 때 다시 구성해 볼 함수
int *make_prices(int n)
{
	int max_gap = 10;
	int min_gap = 1;

	// '일정한 랜덤'이 나오도록 구성하고 있어요.
	// '이제 다른 숫자로 해 볼래' 한다면 아래의 숫자를 다른 거로 바꾸면 돼요.
	srand(796167);


	int *result = malloc(sizeof(int) * (n + 1));
	int idx;

	r = malloc(sizeof(int) * (n + 1));
	idxs = malloc(sizeof(int) * (n + 1));

	for ( idx = 0; idx <= n; ++idx )
	{
		r[idx] = 0;
		idxs[idx] = 0;
	}

	result[0] = 0;
	
	for ( idx = 1; idx <= n; ++idx )
		result[idx] = result[idx - 1] + rand() % (max_gap - min_gap) + min_gap;

	return default_prices;
	//return result;
}


long long call_count;

// p_1 --> prices[1]
int cut_rod_topdown(int *prices, int n)
{
	int max_value;
	int idx;

	call_count += 1;

	// n == 0이면 0,
	if ( n == 0 )
	{
		return 0;
	}

	// n != 0이면 반복 + 재귀 호출을 통해 max 값 찾기
	max_value = -1;

	// 각 마디를 잘라 보며 maximal value를 가늠하기 시작
	// 주: idx를 [0, n) 범위에서 ㄱㄱ할 수도 있긴 할텐데,
	//     현재 코드 구조상으로는 그렇게 하면 무한루프가 떠버려요.
	//     (잘랐을 때 '더 자르기'를 하지 않을 왼쪽 막대의 길이를 idx로 잡는 중이라 그래요)
	for ( idx = 1; idx <= n; ++idx )
	{
		// idx번째 마디를 잘라 보자.
		int current_value = prices[idx] + cut_rod_topdown(prices, n - idx);

		// 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있다면 표시
		if ( current_value > max_value )
		{
			max_value = current_value;
			idxs[n] = idx;
		}
	}

	return max_value;
}


// p_1 --> prices[1]
int cut_rod_topdown_memoized(int *prices, int n)
{
	int max_value;
	int idx;

	call_count += 1;

	// n == 0이면 0,
	if ( n == 0 )
	{
		return 0;
	}

	// 이미 메모가 존재하면 다시 풀지 않을 거임
	if ( r[n] != 0 )
	{
		return r[n];
	}

	// n != 0이면 반복 + 재귀 호출을 통해 max 값 찾기
	max_value = -1;

	// 각 마디를 잘라 보며 maximal value를 가늠하기 시작
	// 주: idx를 [0, n) 범위에서 ㄱㄱ할 수도 있긴 할텐데,
	//     현재 코드 구조상으로는 그렇게 하면 무한루프가 떠버려요.
	//     (잘랐을 때 '더 자르기'를 하지 않을 왼쪽 막대의 길이를 idx로 잡는 중이라 그래요)
	for ( idx = 1; idx <= n; ++idx )
	{
		// idx번째 마디를 잘라 보자.
		int current_value = prices[idx] + cut_rod_topdown_memoized(prices, n - idx);

		// 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있다면 표시
		if ( current_value > max_value )
		{
			max_value = current_value;
			idxs[n] = idx;
		}
	}

	r[n] = max_value;

	return max_value;
}

// p_1 --> prices[1]
int cut_rod_bottomup(int *prices, int n)
{
	int idx_target;

	call_count += 1;

	// n == 0이면 0,
	if ( n == 0 )
	{
		return 0;
	}

	// r_1부터 r_n(포함)까지 채워가기 시작
	for ( idx_target = 1; idx_target <= n; ++idx_target )
	{
		int idx_verify;

		for ( idx_verify = 1; idx_verify <= idx_target; ++idx_verify )
		{
			// idx번째 마디를 잘라 보자.
			int current_value = prices[idx_verify] + r[idx_target - idx_verify];

			// 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있다면 표시
			if ( current_value > r[idx_target] )
			{
				r[idx_target] = current_value;
				idxs[idx_target] = idx_verify;
			}
		}
	}

	return r[n];
}




int main()
{
	int n = 4;
	int *prices = make_prices(n);
	int idx;

	int result;
	double elapsedTime;

	call_count = 0;

	startTime = clock();

	//result = cut_rod_topdown(prices, n);
	//result = cut_rod_topdown_memoized(prices, n);
	result = cut_rod_topdown_memoized(prices, n);

	endTime = clock();

#ifdef WIN32
	elapsedTime = endTime.QuadPart - startTime.QuadPart;
	elapsedTime /= freq.QuadPart;
#else
	elapsedTime = endTime;
	elapsedTime -= startTime;
	elapsedTime /= CLOCKS_PER_SEC;
#endif

	// prices
	printf("p: [");
	for ( idx = 0; idx <= n; ++idx )
		printf("%d, ", prices[idx]);
	printf("\b\b]\n");

	// revenues
	printf("r: [");
	for ( idx = 0; idx <= n; ++idx )
		printf("%d, ", r[idx]);
	printf("\b\b]\n");

	// idxs(왼쪽 작은 막대의 길이)
	printf("i: [");
	for ( idx = 0; idx <= n; ++idx )
		printf("%d, ", idxs[idx]);
	printf("\b\b] - memoized\n");


	// optimal solution(그래서 어디 어디 잘라야 하는지)
	printf("   [");
	while ( n > 0 )
	{
		printf("%d, ", idxs[n]);

		n = n - idxs[n];
	}
	printf("\b\b] - actual\n");

	// value of optimal solution
	printf("r[n]: %d\n", result);

	printf("Elapsed time: %.15f seconds\n", elapsedTime);
	printf("Done.\n");

	return 0;
}