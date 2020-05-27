
#define MAX_NODES	255


typedef struct fcfsArray
{
	struct proc* creationtimes[MAX_NODES];
	int lastIndex;
}fcfsArray;



typedef struct Heap{
	struct proc* prioritys[MAX_NODES];
	int lastIndex;
}Heap;

fcfsArray arr; //for fcfs scheduling

void initFcfsArray(fcfsArray* arr);
void pushCreationTime(fcfsArray* arr, struct proc* creationtime);
void printFcfsArray(fcfsArray arr);
int checkCreationTime(fcfsArray* arr);



Heap heap; //for priority scheduling

void initHeap(Heap* heap);
void pushPriority(Heap* heap, struct proc* priority);
void printHeap(Heap heap);
int checkPriority(Heap* heap);
//int popPriority(Heap* heap);


