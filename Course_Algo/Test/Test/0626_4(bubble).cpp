#include <stdio.h>

#define Compare(a,b) a-b

void Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void Bubble_Sort(int arr[], int length)
{
	int idx_test;
	int isSorted = 0;

	while (isSorted == 0)
	{
		isSorted = 1;

		for (idx_test = 1; idx_test < length; idx_test++)
		{
			
			if (Compare(arr[idx_test - 1], arr[idx_test]) > 0)
			{
				Swap(&arr[idx_test - 1], &arr[idx_test]);

				isSorted = 0;
			}
		}
	}
}

void main()
{
	int arr[6] = {1,4,3,2,5,6};

	Bubble_Sort(arr, sizeof(arr) / sizeof(int));

	printf("[%d %d %d %d %d %d]\n", arr[0], arr[1], arr[2], arr[3],
		arr[4], arr[5]);
}