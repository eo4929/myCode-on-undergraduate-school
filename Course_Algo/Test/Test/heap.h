#pragma once
#define MAX_NODES    255

typedef struct Heap
{
	int prioritys[MAX_NODES];
	int lastIndex;
}Heap;


void initHeap(Heap* heap);
void pushPriority(Heap* heap, int priority);
void printHeap(Heap heap);
int popPriority(Heap* heap); 
