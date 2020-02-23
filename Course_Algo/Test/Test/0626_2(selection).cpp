#include <stdio.h>

#define Compare(l,r) l-r

void Swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void Selection_Sort(int data[],int length)
{
	int i,j; // 교수님이 i,j쓰지 말라시니까 사용하지 말자
	int least;
	int size = length;
	
	for (i = 0; i < size; i++)
	{
		least = i;
		for (j = i + 1; j < size; j++)
		{
			if (Compare(data[j], data[least]) < 0)
				least = j;
		}

		Swap(&data[i], &data[least]);
	}
}

void main()
{
	int list[6] = { 1,4,3,2,5,6 };
	int length = sizeof(list) / sizeof(int);

	Selection_Sort(list,length);

	printf("%d %d %d %d %d %d \n", list[0], list[1], list[2], list[3],
		list[4], list[5]);
}