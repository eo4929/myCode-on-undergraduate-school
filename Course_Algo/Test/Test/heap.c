#include <stdio.h>
#include <conio.h>
#include "heap.h"
#include <math.h>


/*
void initHeap(Heap* heap);
void pushPriority(Heap* heap, int priority);
void printHeap(Heap heap);
int popPriority(Heap* heap);
*/

int main()
{
	Heap heap;

	initHeap(&heap);

	pushPriority(&heap, 0);
	pushPriority(&heap, 0);
	pushPriority(&heap, 0);
	pushPriority(&heap, 13);
	pushPriority(&heap, 4);

	printHeap(heap);
	/*
	int p1 = popPriority(&heap);

	printHeap(heap);

	int p2 = popPriority(&heap);

	printHeap(heap);

	printf("%d %d \n", p1, p2);
	*/
	

	return 0;
}

void initHeap(Heap* heap)
{
	int i;

	for (i = 1; i< MAX_NODES; i++)
		heap->prioritys[i] = 0;

	heap->lastIndex = 0;
}

void pushPriority(Heap* heap, int priority)
{
	int index;

	if (heap->lastIndex == MAX_NODES - 1)
	{
		printf("priority max heap is full\n");
		return;
	}

	heap->lastIndex++;
	index = heap->lastIndex;

	while ((index != 1) && (priority > heap->prioritys[index / 2])) {
		heap->prioritys[index] = heap->prioritys[index / 2];
		index /= 2;
	}

	heap->prioritys[index] = priority;
}

void printHeap(Heap heap)
{
	int i, count, newLineIndex;
	count = 1;
	//newLineIndex = 0;

	for (i = 1; i <= heap.lastIndex; i++)
	{
		printf("%d\t", heap.prioritys[i]);

		//if(pow(2,newLineIndex) == count)
		//{
		//    cprintf("\n");
		//    newLineIndex++;
		//    count = 0;
		//    }

		//    count++;
	}

	printf("\n\n");
}

int popPriority(Heap* heap)
{
	int highestP;
	int temp, parentIndex, childIndex;

	if (heap->lastIndex == 0)
	{
		printf("priority max heap is empty\n");
		return -1;
	}

	highestP = heap->prioritys[1];

	if (heap->lastIndex == 1)
	{
		heap->prioritys[heap->lastIndex] = 0;
		heap->lastIndex = 0;
		return highestP;
	}

	temp = heap->prioritys[heap->lastIndex];
	heap->prioritys[heap->lastIndex] = 0;
	heap->lastIndex--;

	parentIndex = 1;
	childIndex = 2;

	while (childIndex <= heap->lastIndex)
	{
		if (heap->prioritys[childIndex] < heap->prioritys[childIndex + 1])
			childIndex++;

		if (temp > heap->prioritys[childIndex])
			break;

		heap->prioritys[parentIndex] = heap->prioritys[childIndex];
		parentIndex = childIndex;
		childIndex *= 2;
	}

	heap->prioritys[parentIndex] = temp;

	return highestP;
}