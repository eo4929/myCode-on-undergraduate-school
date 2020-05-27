#define MAX_QSIZE 1024
#define MAX_QNUM 3
#define Q0TIMEQUANTUM 2
#define Q1TIMEQUANTUM 4  
#define Q2TIMEQUANTUM 8 
#define QBOOSTTIME 100

typedef struct mlfq{
	struct proc* queue[MAX_QSIZE];
	int level;
	int front;
	int rear;
	int count; // the number of all process counted
	int runnableCount;
	int timeQuantum;
	}mlfq;
	
struct mlfq threeQ[3];
	
int currentQ;
	
void initMlfq(struct mlfq* q);
int is_empty(struct mlfq* q);
int is_full (struct mlfq* q);
int enqueue(struct mlfq* q, struct proc* node);
struct proc* dequeue1(struct mlfq* q);// if SLEEPING proc, enqueue and redequeue
struct proc* dequeue2(struct mlfq* q);// normal dequeue for boosting
void removeProcFromQ(struct proc** p);
struct proc* checkMlfq(struct mlfq* q); // check front proc
struct proc* checkNextMlfq(struct mlfq* q, int index);
void checkRunnableCount(struct mlfq* q); //if ps sleep,delete runnableCount in proc on queue
void boostPriority(void);

//for debugging
void printMlfqAll(void);
