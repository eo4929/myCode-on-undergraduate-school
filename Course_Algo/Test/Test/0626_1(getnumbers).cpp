#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


bool isUnique(int* arr)
{
	int size = sizeof(arr);
	int idx_from, idx_to;

	for (idx_from = 0; idx_from < size; idx_from++)
	{
		for (idx_to = 0; idx_to < size; idx_to++)
		{
			if (idx_from != idx_to && arr[idx_from] == arr[idx_to])
				return false;
		}
	}

	return true;
}

void GetNumbers()
{
	int arr[6] = { 1,1,1,1,1,1 }; // 실수;; arr을 초기화해줘야 함
	int count_loop = 0;

	srand(time(NULL));

	while(isUnique(arr) == false)
	{
		arr[0] = rand() % 46;
		arr[1] = rand() % 46;
		arr[2] = rand() % 46;
		arr[3] = rand() % 46;
		arr[4] = rand() % 46;
		arr[5] = rand() % 46;

		count_loop += 1;
	}

	printf("[%d %d %d %d %d %d]\n", arr[0], arr[1], arr[2], arr[3],
		arr[4], arr[5]);

	printf("count_loop = %d\n", count_loop);
}

void main()
{
	GetNumbers();
}