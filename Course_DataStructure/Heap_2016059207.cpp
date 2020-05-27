#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NODES 50

typedef struct Heap
{
	int nodes[MAX_NODES];
	int lastIndex; // 완전이진트리이므로 필요하다
}Heap;

void initHeap(Heap* heap);
void insertData(Heap* heap, int data);
void printHeap(Heap heap);
void deleteData(Heap* heap);

int main()
{
	Heap heap; // max heap

	initHeap(&heap);

	insertData(&heap, 1);
	insertData(&heap, 3);
	insertData(&heap, 8);
	insertData(&heap, 13);
	insertData(&heap, 4);
	insertData(&heap, 10);
	insertData(&heap, 6);

	printHeap(heap);

	deleteData(&heap);

	printHeap(heap);

	deleteData(&heap);
	printHeap(heap);

	return 0;
}

void initHeap(Heap* heap)
{
	int i;
	for (i = 1; i < MAX_NODES; i++)
		heap->nodes[i] = 0;

	heap->lastIndex = 0;
}
void insertData(Heap* heap, int data)
{
	int index;

	if (heap->lastIndex == MAX_NODES - 1)
	{
		printf("Heap is full\n");
		return;
	}

	//heap->nodes[++heap->lastIndex] = data;
	heap->lastIndex++;
	index = heap->lastIndex;

	//while (heap->nodes[heap->lastIndex/2] <data)
	//{
	//	heap->nodes[heap->lastIndex] = heap->nodes[heap->lastIndex / 2];
	//	heap->nodes[heap->lastIndex / 2] = data;
	//}
	while ((index != 1) && (data > heap->nodes[index / 2]))
	{
		heap->nodes[index] = heap->nodes[index / 2];
		index /= 2;
	}
	heap->nodes[index] = data;
}
void printHeap(Heap heap)
{
	int i, count, newLineIndex;
	count = 1;
	newLineIndex = 0;
	for (i = 1; i <= heap.lastIndex; i++)
	{
		printf("%d\t", heap.nodes[i]);
		// tree의 각 level별 출력
		if (pow(2, newLineIndex) == count) //k레벨 레벨별 노드의 개수 2^k
		{
			printf("\n");
			newLineIndex++;
			count = 0;
		}
		count++;
	}
	printf("\n\n");
}
void deleteData(Heap* heap)
{
	int temp, parentIndex, childIndex;

	if (heap->lastIndex == 0)
	{
		printf("HEap is empty\n");
		return;
	}

	if (heap->lastIndex == 1)
	{
		heap->nodes[heap->lastIndex] = 0;
		heap->lastIndex = 0;
		return;
	}

	temp = heap->nodes[heap->lastIndex];
	heap->nodes[heap->lastIndex] = 0; //
	heap->lastIndex--;

	parentIndex = 1;
	childIndex = 2;

	while (childIndex <= heap->lastIndex)
	{
		if (heap->nodes[childIndex] < heap->nodes[childIndex + 1])
			//heap->nodes[parentIndex] = heap->nodes[childIndex + 1];
			childIndex++;

		if (temp>heap->nodes[childIndex])
			break;

		heap->nodes[parentIndex] = heap->nodes[childIndex];
		parentIndex = childIndex;
		childIndex *= 2;
	}

	heap->nodes[parentIndex] = temp;
	//
}