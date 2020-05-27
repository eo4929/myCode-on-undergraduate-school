/*
	알고리즘및문제해결기법 2주차 예시 파일

	yeshi_3_1bu.c

	2주차 1부 수업시간에 같이 만들어 본 파일에 stack 자료구조와 Quicksort()를 추가하고,
	테스트를 위해 주어진 배열에 임의의 값을 담아 주는 FillRandomly()를 추가하고,
	Compare(), Swap() 호출 횟수를 별도의 global 변수 count_Compare, count_Swap에 담아 두었어요.


	대충 구경해 볼 만한 내용들에 주석을 달아 두고
	// (참고) 와 같은 느낌으로 표시해 두었어요.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 부분 배열의 시작 index와 끝 index 정보를 몰아 담기 위한 구조체 형식입니다.
struct Data_SubArray
{
	// 부분 배열의 0번째 칸이 전체 배열의 몇 번째 칸인지를 나타냅니다.
	int idx_begin;

	// 부분 배열의 length번째 칸이 전체 배열의 몇 번째 칸인지를 나타냅니다.
	// (length번째 칸은 부분 배열에 속하진 않음)
	int idx_end;
};

// 배열을 써서 매우 간단히 구성해 본 stack입니다.
struct Stack
{
	struct Data_SubArray *data;
	struct Data_SubArray *top;
};

// 새 stack을 만들어 return합니다.
// 얘는 basic operation은 아니긴 하지만, 뭐 C++ / Java의 생성자 같은 느낌으로 쓰면 될 듯 해요.
struct Stack *New_Stack()
{
	struct Stack *result = malloc(sizeof(struct Stack));

	// 귀찮으므로 그냥 256칸으로 잡아 둠 -> 어차피 엄청 큰 배열을 sort할 게 아닌 이상 넘칠 일은 없을 듯
	result->data = malloc(sizeof(struct Data_SubArray) * 256);

	// top이 0이면 비어 있는 것
	result->top = 0;
}

// 주어진 stack이 비어 있는지 여부를 return합니다.
// 얘는 준 basic operation이라 할 수 있어요.
int IsEmpty(struct Stack *stack)
{
	return stack->top == 0;
}

// 주어진 stack에 새로운 부분 배열 정보를 올립니다.
// 얘는 basic operation이에요.
void Push(struct Stack *stack, struct Data_SubArray new_data)
{
	// stack이 비어 있지 않다면 현재 top 다음 칸이 새로운 top이 됨
	if ( IsEmpty(stack) == 0 )
		++stack->top;
	// stack이 비어 있다면 미리 잡아 둔 data 배열의 0번째 칸이 top이 됨
	else
		stack->top = stack->data;

	stack->top->idx_begin = new_data.idx_begin;
	stack->top->idx_end = new_data.idx_end;
}

// 주어진 stack의 맨 위에 있는 부분 배열 정보를 꺼내 return합니다.
// 얘는 basic operation이에요.
struct Data_SubArray Pop(struct Stack *stack)
{
	struct Data_SubArray result;

	result.idx_begin = stack->top->idx_begin;
	result.idx_end = stack->top->idx_end;

	// Pop()을 해도 여전히 stack이 비지 않을 예정이라면 '바로 앞 칸'이 새로운 top이 됨
	if ( stack->top != stack->data )
		--stack->top;
	// Pop() 하면 stack이 빌 예정이라면 top을 0으로 잡아 둠
	else
		stack->top = 0;

	// 방금 복사해 둔 결과 정보를 return
	return result;
}

// 사용이 끝난 stack을 해제합니다.
// 얘는 basic operation은 아니긴 하지만, 뭐 C++의 파괴자 같은 느낌으로 쓰면 될 듯 해요.
void Delete_Stack(struct Stack *stack)
{
	free(stack->data);
	free(stack);
}







/*
	컴퓨터 분야에서 '비교'는 보통 뺄셈을 통해 진행해요.

	Sorting problem에서는 '작음'이 '우세'를 의미하며,
	Compare()가 음수를 return하는 것은 'left가 우세'를 의미해요.
*/

int count_Compare;

int Compare(int left, int right)
{
	count_Compare = count_Compare + 1;

	//만약 큰 수가 우세하도록 만들고 싶다면 뭐 반대로 적으면 됨!
	//return right - left;

	return left - right;
}

//fillRandomlly가 사용할 비교(count_Compare를 정확히 재기 위함)
int Compare2(int left, int right) 
{
	return left - right;
}

/*
	좁은 목표인 '원본 배열 자리에 결과 담기'를 위해 쓰이는 보편적인 Swap()이에요.
	뭐 이미 다들 알고 있을 테니 그냥 쓰면 될 듯.
*/

int count_Swap;

void Swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;

	count_Swap = count_Swap + 1;
}



void Selection_Sort(int arr[], int length);
void Insertion_Sort(int arr[], int length);
void Bubble_Sort(int arr[], int length);
void Quicksort(int arr[], int length);

void FillRandomly(int arr[], int length);

// (참고) 위에 선언해 둔, 아래에 정의해 둔 네 sort 함수들 중 하나의 위치를 담기 위한 포인터 변수예요.
//        처음에는 selection sort를 진행하도록 세팅해 놨어요.(다른 함수 이름을 = 오른쪽에 대신 적어 두면 그거 호출 가능!)
void (*fp_Sort_Selected)(int arr[], int length) = Quicksort;

int main()
{
	// Data 구성
	int arr[6];
	int length = sizeof arr / sizeof(int);

	FillRandomly(arr, length);
	//arr[0] = 6; arr[1] = 5; arr[2] = 4; arr[3] = 3; arr[4] = 2; arr[5] = 1; 
	//arr[0] = 1; arr[1] = 2; arr[2] = 3; arr[3] = 4; arr[4] = 5; arr[5] = 6; 


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

	printf("Compare: %d times, Swap: %d times\n", count_Compare, count_Swap);


	return 0;
}

void Selection_Sort(int arr[], int length)
{
	int idx_target;

	// 0등부터 n등까지 각각의 자리에 대해...
	for ( idx_target = 0; idx_target < length; ++idx_target ) // n번 호출
	{
		// 적임자 찾기(얘가 맞다고 가정하고 그게 맞나 검증)
		int idx_god = idx_target;
		int idx_challenger;


		for ( idx_challenger = idx_god + 1; idx_challenger < length; ++idx_challenger )
		{
			// 가정이 틀렸다면 말 바꾸기
			if ( Compare(arr[idx_god], arr[idx_challenger]) > 0 ) // n-1 , n-2 ... -> 그냥 n번
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
				break; // 베스트 케이스일때 1,2,3,4,5,6 들어있다면 안에있는 for문은 한번씩만 확인함! O(n)밖에 안됨!!!
						// 워스트 케이스는 6,5,4,3,2,1 이면 O(n^2)이다!!

	}
}

void Bubble_Sort(int arr[], int length)
{
	int idx_test;
	int isSorted = 0;

	// Sort가 다 되었다고 확신할 수 있을 때까지 반복(사실 최대 횟수가 가시적이긴 한데 요건 내일 ㄱㄱ)
	while ( isSorted == 0 )
	{
		// 일단 가정 후 검증
		isSorted = 1;

		for ( idx_test = 1; idx_test < length; ++idx_test ) // 이것도 워스트는 n^2 , 베스트는 n(while이 1번 밖에 안도니까!!)
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

		// 만약 단 하나의 모순도 발견되지 않았다면 isSorted 값은 여전히 1일 것이고,
		// 그렇다면 sort가 완료된 것이므로 반복을 끝내도 됨 --> 끝내게 됨
	}
}

/* 이전버전

*/

void Quicksort(int arr[], int length) // 워스트는 기준값이 계속 맨 오른쪽일 경우!! : left가 기준값까지 가버리니 n^2이다
{
	// Data 선언

	// 큰 반복들 사이에서 data를 주고받기 위한 stack입니다.
	struct Stack *stack = New_Stack();

	// 이번 반복에서 다룰 부분 배열에 대한 정보를 stack에서 Pop()해 와 담을 변수입니다.
	struct Data_SubArray data_current;

	// 이번 반복에서 만든 왼쪽 부분 배열에 대한 정보를 stack에 Push()하기 위해 임시로 담아 둘 변수입니다.
	struct Data_SubArray data_left_subArray;

	// 이번 반복에서 만든 오른쪽 부분 배열에 대한 정보를 stack에 Push()하기 위해 임시로 담아 둘 변수입니다.
	struct Data_SubArray data_right_subArray;



	// Initialization: 첫 반복은 '전체 배열'을 다루어야 하므로 해당 정보를 stack에 Push()해 둠
	data_current.idx_begin = 0;
	data_current.idx_end = length;
	Push(stack, data_current);

	// Termination: stack에 더 이상 'sort할 부분 배열 정보'가 없으면 탈출
	while (IsEmpty(stack) == 0)
	{
		// Data 선언
		int idx_left;
		int idx_mid;
		int idx_begin;
		int idx_end;

		// 이번 반복에 쓸 정보 Pop()해 오기
		data_current = Pop(stack);

		// Pop한 정보를 토대로 좀 더 읽기 쉬운 변수들에 옮겨 담음
		idx_begin = data_current.idx_begin;
		idx_end = data_current.idx_end;

		// 영역을 세 부분으로 나눌 예정이므로 idx가 두 개 더 필요
		idx_left = idx_begin;
		idx_mid = idx_end - 2;

		// Maintenance
		while (idx_left <= idx_mid)
		{
			// 모순이 발생했나 검사
			if (Compare(arr[idx_left], arr[idx_end - 1]) > 0)
			{
				// 발생했다면 얘는 기준 숫자보다 크다. --> 최종적으로 오른쪽 영역에 있어야 함.
				Swap(&arr[idx_left], &arr[idx_mid]);
				idx_mid -= 1;
			}
			else
			{
				// 발생하지 않았다면 얘는 여기 두고 다음 친구를 검사하기 시작
				idx_left += 1;
			}
		}

		// 기준 숫자를 적절한 위치에 Swap()
		Swap(&arr[idx_left], &arr[idx_end - 1]);




		// 다음 반복에 쓰게 될 정보 담아 놓기
		if (idx_left - idx_begin > 1)
		{
			data_left_subArray.idx_begin = idx_begin;
			data_left_subArray.idx_end = idx_left;

			Push(stack, data_left_subArray);
		}

		if (idx_end - idx_left > 2)
		{
			data_right_subArray.idx_begin = idx_left + 1;
			data_right_subArray.idx_end = idx_end;

			Push(stack, data_right_subArray);
		}

	}

	// Sort가 다 끝났다면 stack 반납 후 종료
	Delete_Stack(stack);
}

/*
// 공간복잡도 고려한 퀵정렬
void Quicksort(int arr[], int length) // 워스트는 기준값이 계속 맨 오른쪽일 경우!! : left가 기준값까지 가버리니 n^2이다
{
// Data 선언

// 큰 반복들 사이에서 data를 주고받기 위한 stack입니다.
struct Stack *stack = New_Stack();

// 이번 반복에서 다룰 부분 배열에 대한 정보를 stack에서 Pop()해 와 담을 변수입니다.
struct Data_SubArray data_current;

// 이번 반복에서 만든 왼쪽 부분 배열에 대한 정보를 stack에 Push()하기 위해 임시로 담아 둘 변수입니다.
struct Data_SubArray data_left_subArray;

// 이번 반복에서 만든 오른쪽 부분 배열에 대한 정보를 stack에 Push()하기 위해 임시로 담아 둘 변수입니다.
struct Data_SubArray data_right_subArray;

int lengthOfLeftSubArray; // 왼쪽 부분 배열의 길이 계산하기 위한 변수
int lengthOfrightSubArray;

// Initialization: 첫 반복은 '전체 배열'을 다루어야 하므로 해당 정보를 stack에 Push()해 둠
data_current.idx_begin = 0;
data_current.idx_end = length;
Push(stack, data_current);

// Termination: stack에 더 이상 'sort할 부분 배열 정보'가 없으면 탈출
while ( IsEmpty(stack) == 0 )
{
// Data 선언
int idx_left;
int idx_mid;
int idx_begin;
int idx_end;

// 이번 반복에 쓸 정보 Pop()해 오기
data_current = Pop(stack);

// Pop한 정보를 토대로 좀 더 읽기 쉬운 변수들에 옮겨 담음
idx_begin = data_current.idx_begin;
idx_end = data_current.idx_end;

// 영역을 세 부분으로 나눌 예정이므로 idx가 두 개 더 필요
idx_left = idx_begin;
idx_mid = idx_end - 2;

// Maintenance
while ( idx_left <= idx_mid )
{
// 모순이 발생했나 검사
if ( Compare(arr[idx_left], arr[idx_end - 1]) > 0 )
{
// 발생했다면 얘는 기준 숫자보다 크다. --> 최종적으로 오른쪽 영역에 있어야 함.
Swap(&arr[idx_left], &arr[idx_mid]);
idx_mid -= 1;
}
else
{
// 발생하지 않았다면 얘는 여기 두고 다음 친구를 검사하기 시작
idx_left += 1;
}
}

// 기준 숫자를 적절한 위치에 Swap()
Swap(&arr[idx_left], &arr[idx_end - 1]);

// 양쪽 부분 배열의 길이 구함
lengthOfLeftSubArray = idx_left - idx_begin;
lengthOfrightSubArray = idx_end - idx_left -1;

// 왼쪽 부분 배열 길이가 더 크면 나중에 sort하기 위해 먼저 push
if (lengthOfLeftSubArray > lengthOfrightSubArray)
{
// 다음 반복에 쓰게 될 정보 담아 놓기
if (idx_left - idx_begin > 1)
{
data_left_subArray.idx_begin = idx_begin;
data_left_subArray.idx_end = idx_left;

Push(stack, data_left_subArray);
}

if (idx_end - idx_left > 2)
{
data_right_subArray.idx_begin = idx_left + 1;
data_right_subArray.idx_end = idx_end;

Push(stack, data_right_subArray);
}
}
else
{
// 다음 반복에 쓰게 될 정보 담아 놓기
if (idx_end - idx_left > 2)
{
data_right_subArray.idx_begin = idx_left + 1;
data_right_subArray.idx_end = idx_end;

Push(stack, data_right_subArray);
}

if (idx_left - idx_begin > 1)
{
data_left_subArray.idx_begin = idx_begin;
data_left_subArray.idx_end = idx_left;

Push(stack, data_left_subArray);
}

}




}

// Sort가 다 끝났다면 stack 반납 후 종료
Delete_Stack(stack);
}
*/
// (참고)파일 위에 있는 stack 관련 코드들을 구경해 보고 오면 좋겠어요



void FillRandomly(int arr[], int length)
{
	// Bogosort에서와 비슷한 느낌으로, 겹치지 않는 숫자 조합이 나올 때까지 계속 새로 만들 예정이에요.

	int max_number = length * 8;
	int isUnique;

	srand(time(0));


	// 일단 최소 한 번은 뽑아야 하니 '겹침 == 반복 더 해야 함'으로 설정
	isUnique = 0;

	// 겹치지 않는 숫자 조합을 만들 때까지 반복
	while ( isUnique == 0 )
	{
		int idx_target = 0;
		int idx_verify = 0;

		// 일단 긍정적인 가정
		isUnique = 1;
		
		// 가정에 대한 검증
		while ( idx_target < length )
		{
			arr[idx_target] = rand() % max_number;

			idx_verify = 0;

			while ( idx_verify < idx_target )
			{
				// '같음' 비교도 미리 만들어 둔 Compare()를 사용하고 있어요.
				if ( Compare2(arr[idx_verify], arr[idx_target]) == 0 )
				{
					// 하나라도 같다면 더 이상의 검증은 불필요
					isUnique = 0;
					break;
				}

				idx_verify = idx_verify + 1;
			}

			// 하나라도 같다면 더 이상의 숫자 뽑기는 불필요
			if ( isUnique == 0 )
			{
				break;
			}

			idx_target = idx_target + 1;
		}


	}
}
