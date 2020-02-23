

#include "heap.h"


void 
initFcfsArray(fcfsArray* arr){
	int i;

	for(i=1; i< MAX_NODES; i++)
		arr->creationtimes[i] = 0;

	arr->lastIndex = 0;
}

void 
pushCreationTime(fcfsArray* arr, struct proc* creationtime){
	int index;

	if(arr->lastIndex == MAX_NODES - 1){
		cprintf("fcfsArray is full\n");
		return;
	}

	arr->lastIndex++;
	index = arr->lastIndex;

	while( (index !=1) && (creationtime->creationtime < arr->creationtimes[index / 2]->creationtime)){
		arr->creationtimes[index] = arr->creationtimes[index / 2];
		index /= 2;
	}

	arr->creationtimes[index] = creationtime;
}

void 
printFcfsArray(fcfsArray arr){
	int i;

	for(i=1; i<= arr.lastIndex; i++){
		cprintf("%d\t",arr.creationtimes[i]->creationtime);
	}

	cprintf("\n\n");
}

int 
checkCreationTime(fcfsArray* arr){
	return arr->creationtimes[1]->creationtime;
}



void 
initHeap(Heap* heap){
	int i;

	for(i=1; i< MAX_NODES; i++)
		heap->prioritys[i] = 0;

	heap->lastIndex = 0;
}

void 
pushPriority(Heap* heap, struct proc* priority){
	int index;

	if(heap->lastIndex == MAX_NODES - 1){
		cprintf("priority max heap is full\n");
		return;
	}

	heap->lastIndex++;
	index = heap->lastIndex;

	while( (index !=1) && (priority->priority > heap->prioritys[index / 2]->priority)){
		heap->prioritys[index] = heap->prioritys[index / 2];
		index /= 2;
	}

	heap->prioritys[index] = priority;
}

void 
printHeap(Heap heap){
	int i;

	for(i=1; i<= heap.lastIndex; i++){
		cprintf("%d\t",heap.prioritys[i]->priority);
	}

	cprintf("\n\n");
}

int 
checkPriority(Heap* heap){
	return heap->prioritys[1]->priority;
}















/*
int 
popPriority(Heap* heap){
	int highestP;
	int temp, parentIndex, childIndex;

	if(heap->lastIndex == 0){
		cprintf("priority max heap is empty\n");
		return -1;
	}

	highestP = heap->prioritys[1]->priority;

	if(heap->lastIndex == 1){
		heap->prioritys[heap->lastIndex] = 0;
		heap->lastIndex = 0;
		return highestP;
	}

	temp = heap->prioritys[heap->lastIndex]->priority;
	heap->prioritys[heap->lastIndex] = 0;
	heap->lastIndex--;

	parentIndex = 1;
	childIndex = 2;

	while(childIndex <= heap->lastIndex){
		if(heap->prioritys[childIndex]->priority < heap->prioritys[childIndex + 1]->priority)
			childIndex++;

		if(temp > heap->prioritys[childIndex]->priority)
			break;

		heap->prioritys[parentIndex] = heap->prioritys[childIndex];
		parentIndex = childIndex;
		childIndex *= 2;
	}

	heap->prioritys[parentIndex]->priority = temp;

	return highestP;
}
*/
