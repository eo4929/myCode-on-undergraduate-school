/*
	알고리즘및문제해결기법 2주차 예시 파일

	yeshi_2_1bu.c

	1부 수업시간에 같이 만들어 본 파일이에요.

	대충 구경해 볼 만한 내용들에 주석을 달아 두고
	// (참고) 와 같은 느낌으로 표시해 두었어요.
*/

#include <stdio.h>
#include <stack>
/*
	컴퓨터 분야에서 '비교'는 보통 뺄셈을 통해 진행해요.

	Sorting problem에서는 '작음'이 '우세'를 의미하며,
	Compare()가 음수를 return하는 것은 'left가 우세'를 의미해요.
*/
int Compare(int left, int right)
{
	//만약 큰 수가 우세하도록 만들고 싶다면 뭐 반대로 적으면 됨!
	//return right - left;

	return left - right;
}


/*
	좁은 목표인 '원본 배열 자리에 결과 담기'를 위해 쓰이는 보편적인 Swap()이에요.
	뭐 이미 다들 알고 있을 테니 그냥 쓰면 될 듯.
*/
void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}


void Selection_Sort(int arr[], int length);
void Insertion_Sort(int arr[], int length);
void Bubble_Sort(int arr[], int length);
void Quicksort(int arr[], int length);

// (참고) 위에 선언해 둔, 아래에 정의해 둔 세 sort 함수들 중 하나의 위치를 담기 위한 포인터 변수예요.
//        처음에는 selection sort를 진행하도록 세팅해 놨어요.(다른 함수 이름을 = 오른쪽에 대신 적어 두면 그거 호출 가능!)
void (*fp_Sort_Selected)(int arr[], int length) = Bubble_Sort;

int main()
{
	// Data 구성(수동으로 했어요)
	int arr[] = { 6, 5, 4, 3, 2, 1 };
	int length = sizeof arr / sizeof(int);

	// 출력할 때 index 담을 변수
	int idx;

	printf("Before: [");
	for ( idx = 0; idx < length; ++idx )
		printf("%d, ", arr[idx]);
	printf("\b\b]\n"); // (참고) '\b'를 쓰면 '백스페이스 글자'를 print함으로써 글자를 지울 수 있어요!

	// main() 위에서 선언 & 초기화해 둔 포인터 변수를 써서 '세팅해 둔 sort 함수'를 호출하고 있어요.
	fp_Sort_Selected(arr, length);

	printf("After:  [");
	for ( idx = 0; idx < length; ++idx )
		printf("%d, ", arr[idx]);
	printf("\b\b]\n");


	return 0;
}

void Selection_Sort(int arr[], int length)
{
	int idx_target;

	// 0등부터 n등까지 각각의 자리에 대해...
	for ( idx_target = 0; idx_target < length; ++idx_target )
	{
		// 적임자 찾기(얘가 맞다고 가정하고 그게 맞나 검증)
		int idx_god = idx_target;
		int idx_challenger;

		for ( idx_challenger = idx_god + 1; idx_challenger < length; ++idx_challenger )
		{
			// 가정이 틀렸다면 말 바꾸기
			if ( Compare(arr[idx_god], arr[idx_challenger]) > 0 )
			{
				idx_god = idx_challenger;
			}
		}

		// Swap()
		Swap(&arr[idx_target], &arr[idx_god]);
	}
}


void Insertion_Sort(int arr[], int length)
{
	int idx_new = 1;

	// 새로운 한 칸을 포함하는 'sorted 영역' 보장하기
	for ( idx_new = 1; idx_new < length; ++idx_new )
	{
		int idx_target = idx_new;

		// 새 한 칸에 앉았던 숫자가 '어디까지 올라가야' sorted 상태라고 보장할 수 있나 체크
		for ( idx_target = idx_new; idx_target > 0; --idx_target )
			if ( Compare(arr[idx_target - 1], arr[idx_target]) > 0 )
				Swap(&arr[idx_target - 1], &arr[idx_target]);
			else
				break;

	}
}

void Bubble_Sort(int arr[], int length)
{
	int idx_test;
	int isSorted = 0;
	//int idx_sorted = length; //큰 반복 1회하면 마지막 배열은 sorted가 된 것이니 버블에서 compare를 줄일 수 있음

	// Sort가 다 되었다고 확신할 수 있을 때까지 반복(사실 최대 횟수가 가시적이긴 한데 요건 내일 ㄱㄱ)
	while ( isSorted == 0 )
	{
		// 일단 가정 후 검증
		isSorted = 1;

		for ( idx_test = 1; idx_test < length; ++idx_test )
		//for (idx_test = 1; idx_test < idx_sorted; ++idx_test) //큰 반복 1회하면 마지막 배열은 sorted가 된 것이니 버블에서 compare를 줄일 수 있음
		{
			// 모순(left > right)이 보인다면...
			if ( Compare(arr[idx_test - 1], arr[idx_test]) > 0 )
			{
				// 그 둘을 Swap()해서 눈 앞에 보이는 모순을 제거
				Swap(&arr[idx_test - 1], &arr[idx_test]);

				// 일단 이번 반복은 글렀으므로 표시해 둠(적어도 다음 반복은 반드시 수행될 것임)
				isSorted = 0;
			}
		}
		//idx_sorted--; //큰 반복 1회하면 마지막 배열은 sorted가 된 것이니 버블에서 compare를 줄일 수 있음 

		// 만약 단 하나의 모순도 발견되지 않았다면 isSorted 값은 여전히 1일 것이고,
		// 그렇다면 sort가 완료된 것이므로 반복을 끝내도 됨 --> 끝내게 됨
	}
}

struct Data_SubArray
{
	int idx_begin; //자료구조로써 배열을 사용하기 위해 시작점과
	int idx_end; // 길이를 알아야 함
};

void Quicksort(int arr[], int length)
{
	std::stack<Data_SubArray> stack;
	Data_SubArray data_current;
	Data_SubArray data_left_subArray;
	Data_SubArray data_right_subArray;

	data_current.idx_begin = 0;
	data_current.idx_end = length;

	stack.push(data_current); // initialize // 전체를 sort하기 위함

	while (stack.empty() == false) // termination // 아무리 많이 돌아도 n번 돈다
	{
		//data 선언
		int idx_left;
		int idx_mid;
		int idx_begin;
		int idx_end;

		// 이번 반복에서 쓸 정보 가져오기
		data_current = stack.top();
		idx_begin = data_current.idx_begin;
		idx_end = data_current.idx_end;
		stack.pop();

		idx_left = idx_begin;
		idx_mid = idx_end-2; //end는 길이이므로 1개 빼고 맨 오른쪽은 기준값이니 또 1개 뺀다

		// maintenance // 정확히 무엇을 해야 하는지가 중요!!! 잘 공부해 둘것
		
		while (idx_left <= idx_mid)
		{
			// 모순 발생했는지 검사
			if (Compare(arr[idx_left], arr[idx_end - 1]) > 0)
			{
				// 발생하면(기준 숫자보다 크다 -> 오른쪽 영역에 있어야함)
				Swap(&arr[idx_left], &arr[idx_mid]);
				idx_mid -= 1;
			}
			else
			{
				// 발생하지 않았다면 다음 놈을 검사하기 시작
				idx_left += 1;
			}
		}

		// 기준 숫자를 적절한 위치에 Swap
		Swap(&arr[idx_left], &arr[idx_end - 1]);

		//다음 반복에 쓰게 될 정보 담아 놓기

		if (idx_left - idx_begin > 1)
		{
			data_left_subArray.idx_begin = idx_begin;
			data_left_subArray.idx_end = idx_left;

			stack.push(data_left_subArray);
		}

		if (idx_end - idx_left > 2)
		{
			data_right_subArray.idx_begin = idx_left + 1;
			data_right_subArray.idx_end = idx_end;

			stack.push(data_right_subArray);
		}

	}
}