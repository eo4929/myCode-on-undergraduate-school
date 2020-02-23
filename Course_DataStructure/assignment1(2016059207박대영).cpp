#include <stdio.h>
#define SIZE 10

int list[SIZE] = { 38,27,1,43,3,9,82,10,16,60 };

void selectionSort(int * list, int n)
{

	int i, j, least, temp;
	
	for(i=0; i<n-1; i++)
	{
		least = i;
		for(j = i+1; j<n; j++)
			if(list[j] < list[least]) least = j;
		
		temp = list[least];
		list[least] = list[i];
		list[i] = temp;
	}

}

int main()
{
	int i;

	printf("Before Sorting\n");
	for(i=0; i<SIZE; i++)
		printf("%d ", list[i]);
	printf("\n");

	selectionSort(list, SIZE);

	printf("After Sorting\n");
	for(i=0; i<SIZE; i++)
		printf("%d ", list[i]);
	printf("\n");

		
	return 0;

}