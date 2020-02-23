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

int *r; // r_0 ~ r_n까지 담을 예정 //버전2와 버전3에서 사용
int *idxs; //step4을 위함

// 새 가격표를 만들고 싶을 때 다시 구성해 볼 함수
// 지금은 그냥 기본 가격표만 return하도록 만들어 놓았어요.
int *make_prices(int n)
{
	int max_gap = 10; //랜덤한 가격표를 initialize하는 부분 
	int min_gap = 1;

	int *result = malloc(sizeof(int) * (n + 1));
	int idx;

	r = malloc(sizeof(int) * (n + 1));
	idxs = malloc(sizeof(int) * (n + 1));


	for (idx = 0; idx <= n; ++idx)
	{
		r[idx] = 0; //무조건 갱신되도록 작은 값 담아놓기 + 어차피 r[0]는 0이므로
		idxs[idx] = 0;
	}

	result[0] = 0; //길이가 0이면 0원(value가 0)

	for (idx = 1; idx <= n; ++idx)
	{
		result[idx] = result[idx - 1] + rand() % (max_gap-min_gap); 
		// 길이가 증가함에 따른 단조증가의 가격추세를 반영하기 위함
	}

	//return result;  //랜덤한 가격표
	return default_prices; //기존 가격표
}


long long call_count;

// p_1 --> prices[1]
int cut_rod_topdown(int *prices, int n) //prices는 가격표
{
	int max_value;
	int idx;

	call_count += 1;

	// 길이가 0이면 즉, n == 0이면 value 0 반환 
	if (n == 0)
		return 0;

	// n != 0이면 반복 + 재귀 호출을 통해 max 값 찾기
	max_value = -1; // 절대 성립하지 않을 value로 가정
	
	for (idx = 1; idx <= n; ++idx) // max 구하는 부분 // idx=0부터 하면 안됨 주의!!
	{
		//idx번째 마디를 잘라보자!
		int current_value = prices[idx] + cut_rod_topdown(prices,n-idx); //pi + rn-i 부분
							//왼쪽부분(정찰제로 매기고) 오른쪽은 다시 구할 것임

		if (current_value > max_value) // 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있음을 표시
		{
			max_value = current_value;
			idxs[n] = idx; //step4용
		}
	}

	return max_value;
}

int cut_rod_topdown_memorized(int *prices, int n) //memorized top-down 방식
{
	int max_value;
	int idx;

	call_count += 1;

	// n == 0이면 0,
	if (n == 0)
		return 0;

	if (r[n] != 0)
	{
		return r[n];
	}

	// n != 0이면 반복 + 재귀 호출을 통해 max 값 찾기
	max_value = -1; // 가정

	for (idx = 1; idx <= n; ++idx) // max 구하는 부분
	{
		//idx번째 마디를 잘라보자!
		int current_value = prices[idx] + cut_rod_topdown_memorized(prices, n - idx);
		//왼쪽부분(정찰제로 매기고) 오른쪽은 다시 구할 것임

		if (current_value > max_value) // 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있음을 표시
		{
			idxs[n] = idx;

			max_value = current_value;
		}
	}

	r[n] = max_value;

	return max_value;
}

int cut_rod_bottomup(int *prices, int n) //bottop-up 방식
{
	int max_value;
	int idx_target;

	call_count += 1;

	// n == 0이면 0,
	if (n == 0)
		return 0;

	max_value = -1;

	for (idx_target = 1; idx_target <= n; ++idx_target)
	{
		int idx_verify;

		for (idx_verify = 1; idx_verify <= idx_target; ++idx_verify)
		{
			int current_value = prices[idx_verify] + r[idx_target - idx_verify];

			if (current_value > r[idx_target]) // 이렇게 자르는 게 현재까지는 가장 많은 돈을 벌 수 있음을 표시
			{
				max_value = current_value;

				r[idx_target] = max_value;
				idxs[idx_target] = idx_verify;
			}
		}
		
	}
	return max_value;
}


int main()
{
	int n = 3; // 1 2 3 4 .. 10 다 확인해보자
	int *prices = make_prices(n);

	int result;
	double elapsedTime;

	// '일정한 랜덤'이 나오도록 구성하고 있어요.
	// '이제 다른 숫자로 해 볼래' 한다면 아래의 숫자를 다른 거로 바꾸면 돼요.
	srand(796167);

	call_count = 0;

	startTime = clock();

	result = cut_rod_topdown_memorized(prices, n);

	endTime = clock();

#ifdef WIN32
	elapsedTime = endTime.QuadPart - startTime.QuadPart;
	elapsedTime /= freq.QuadPart;
#else
	elapsedTime = endTime;
	elapsedTime -= startTime;
	elapsedTime /= CLOCKS_PER_SEC;
#endif

	 //step4를 구현하는 코드
	/*
	while (n > 0)
	{
		printf("%d, ", idxs[n]);

		n = n - idxs[n];
	}

	printf("\b\b\n");
	*/
	
	
	
	
	printf("cut_rod(%d) -> %d\n"
		   "Call count: %lld\n"
		   "Elapsed time: %.15f seconds\n", n, result, call_count, elapsedTime);
		   
	printf("Done.\n");
	
	

	return 0;
}