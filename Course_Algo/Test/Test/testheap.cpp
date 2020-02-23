#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES    255

typedef struct proc {
	int priority; // ps priority
}proc;


typedef struct Heap {
	struct proc* prioritys[MAX_NODES];
	int lastIndex;
}Heap;


Heap heap; //assignment2

proc* p1;
proc* p2;
proc* p3;

void initHeap(Heap* heap);
void pushPriority(Heap* heap, struct proc* priority);
void printHeap(Heap heap);
int checkPriority(Heap* heap);
int popPriority(Heap* heap);


int main()
{
	p1 = (proc *)malloc(sizeof(proc));
	p2 = (proc *)malloc(sizeof(proc));
	p3 = (proc *)malloc(sizeof(proc));

	p1->priority = 1;
	p2->priority = 2;
	p3->priority = 3;

	initHeap(&heap);

	pushPriority(&heap, p1);
	pushPriority(&heap, p2);
	pushPriority(&heap, p3);

	printHeap(heap);

	int pr2 = popPriority(&heap);
	printf("\n%d\n", pr2);

	printHeap(heap);
	return 0;
}


void
initHeap(Heap* heap) {
	int i;

	for (i = 1; i< MAX_NODES; i++)
		heap->prioritys[i] = 0;

	heap->lastIndex = 0;
}

void
pushPriority(Heap* heap, struct proc* priority) {
	int index;

	if (heap->lastIndex == MAX_NODES - 1) {
		printf("priority max heap is full\n");
		return;
	}

	heap->lastIndex++;
	index = heap->lastIndex;

	while ((index != 1) && (priority->priority > heap->prioritys[index / 2]->priority)) {
		heap->prioritys[index] = heap->prioritys[index / 2];
		index /= 2;
	}

	heap->prioritys[index] = priority;
}

void
printHeap(Heap heap) {
	int i;

	for (i = 1; i <= heap.lastIndex; i++) {
		printf("%d\t", heap.prioritys[i]->priority);
	}

	printf("\n\n");
}

int
checkPriority(Heap* heap) {
	return heap->prioritys[1]->priority;
}

int
popPriority(Heap* heap) {
	int highestP;
	//int temp, parentIndex, childIndex;
	proc* temp;
	int parentIndex, childIndex;

	if (heap->lastIndex == 0) {
		printf("priority max heap is empty\n");
		return -1;
	}

	highestP = heap->prioritys[1]->priority;

	if (heap->lastIndex == 1) {
		heap->prioritys[heap->lastIndex] = NULL;
		heap->lastIndex = 0;
		return highestP;
	}

//	temp = heap->prioritys[heap->lastIndex]->priority;
	temp = heap->prioritys[heap->lastIndex];
//	heap->prioritys[heap->lastIndex] = 0;
	heap->prioritys[heap->lastIndex] = NULL;
	heap->lastIndex--;

	parentIndex = 1;
	childIndex = 2;
	
	while (childIndex <= heap->lastIndex) {
		if (heap->prioritys[childIndex]->priority < heap->prioritys[childIndex + 1]->priority)
			childIndex++;
		    //tempÀÇ pr!
		if (temp->priority > heap->prioritys[childIndex]->priority)
			break;
		
		heap->prioritys[parentIndex] = heap->prioritys[childIndex];
		parentIndex = childIndex;
		childIndex *= 2;
	}
	// º¯°æ!
	heap->prioritys[parentIndex] = temp;

	return highestP;
}