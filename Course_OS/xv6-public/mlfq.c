#include "mlfq.h"
	
void 
initMlfq(struct mlfq* q)
{
	int i;

	q->front = 0;
	q->rear = 0;
	q->count = 0;
	q->runnableCount = 0;

	for(i=0; i<MAX_QSIZE; i++){
		removeProcFromQ(&q->queue[i]);
	}

	threeQ[0].level = 0;
	threeQ[1].level = 1;
	threeQ[2].level = 2;

	threeQ[0].timeQuantum = Q0TIMEQUANTUM;
	threeQ[1].timeQuantum = Q1TIMEQUANTUM;
	threeQ[2].timeQuantum = Q2TIMEQUANTUM;

}

int 
is_empty(struct mlfq* q) // if front = rear , queue is empty
{
	if(q->front == q->rear && q->count == 0 ){
		cprintf("Queue is empty\n");
		return 1;
	}
	else return 0;
	
}

int
is_full(struct mlfq* q) // if front = rear+1, q is full
{
	if((q->rear)%MAX_QSIZE == q->front && q->count == MAX_QSIZE){
		cprintf("Queue is full\n");
	    return 1;
	}
	else return 0;
}

int
enqueue( struct mlfq* q, struct proc* node )
{
	if(is_full(q))
		return -1;

	q->rear = (q->rear + 1)% MAX_QSIZE;
	q->queue[q->rear] = node;
	node->qlevel = q->level;
	q->count++;
	
	node->qtime =0;
	
	return 0;
}
	
	
struct proc*
dequeue1(struct mlfq* q)
{
	struct proc* ret;
	
	if(is_empty(q)) 
		return 0;

	    // checks if q is full of SLEEPING proc or empty
	checkRunnableCount(q);

	if(q->runnableCount == 0) // not exec dequeue
		return 0;
	
	q->front = (q->front + 1)% MAX_QSIZE;
	ret = q->queue[q->front];
	q->count--;

	removeProcFromQ(&q->queue[q->front]);

	if(ret->state != RUNNING ){
		enqueue(q,ret);     //enqueue SLEEPING proc to rear
		dequeue1(q); //dequeue other proc with not SLEEPING state
	}
	
	return ret;
}

struct proc*
dequeue2(struct mlfq* q)
{
	struct proc* ret;
	
	if(is_empty(q))
		return 0;
	
	q->front = (q->front + 1)% MAX_QSIZE;
	ret = q->queue[q->front];
	q->count--;
	
	removeProcFromQ(&q->queue[q->front]);
	
	return ret;
}

void
removeProcFromQ(struct proc** p){
	if(*p) 
		*p=0;
}

struct proc*
checkMlfq(struct mlfq* q)
{
	struct proc* ret;
	ret = q->queue[(q->front+1) % MAX_QSIZE];

	return ret;
}
	    
	
struct proc*
checkNextMlfq(struct mlfq* q, int index)
{
	struct proc* ret;
	ret = q->queue[(q->front+1+index) % MAX_QSIZE];
	
	return ret;
}
	
void
checkRunnableCount(struct mlfq* q)
{
	int i = 0;
	struct proc* p;
	
	q->runnableCount = q->count;
	
	//check how many procs are RUNNABLE or RUNNING state
	for(p = checkMlfq(q); (int)p; p = checkNextMlfq(q,i)){
		if(p->state != RUNNING && p->state != RUNNABLE){
			q->runnableCount--;
		}
		
		i++;
	}
}

void
boostPriority(void)
{
	int i =0;
	struct proc* p;
	int lev0full = 0; // -1 means full
	int lev1full = 0;

	for(p = checkMlfq(&threeQ[1]); (int)p; p = checkNextMlfq(&threeQ[1],i)){
		
		dequeue2(&threeQ[1]);
		lev0full = enqueue(&threeQ[0],p);

		if(lev0full == -1){
			lev1full = enqueue(&threeQ[1],p);
			if(lev1full == -1)
				enqueue(&threeQ[2],p);
		}

		i++;
	}

	i=0;

	for(p = checkMlfq(&threeQ[2]); (int)p; p = checkNextMlfq(&threeQ[2],i)){

		dequeue2(&threeQ[2]);
		lev0full = enqueue(&threeQ[0],p);

		if(lev0full == -1){
			lev1full = enqueue(&threeQ[1],p);
			if(lev1full == -1)
				enqueue(&threeQ[2],p);
		}

	}
}

/*
void
boostPriority(void)
{
	int i = 0, j = 1, k = 0;
	int qCount;
	struct proc* p;
	
	while(j<MAX_QNUM){
		
		qCount = threeQ[j].count;
	        
		for(p = checkMlfq(&threeQ[j]); (int)p; p = checkNextMlfq(&threeQ[j],i)){
			
			k = 0;

			if(qCount == 0) 
				break;
			
			if(p && p->state != UNUSED){
				
				dequeue2(&threeQ[j]);
	            qCount--;

				while(enqueue(&threeQ[k],p) == -1 && k < MAX_QNUM){
	                     k++; // if lev0Queue is full, p move to lev1Queue
	                }
	
	            }
			
			i++; //approach next proc
	        }
		
		i=0; // for moving to lev2 Q
		j++; // for moving to lev2 Q
	}
}
*/
void printMlfqAll(void)
{
	int i=0, j=0;

	for(i=0; i<MAX_QNUM;i++){
		cprintf("\nlev%d Q---------------------------\n",i);

		for(j=0; j<MAX_QSIZE; j++){
			if(threeQ[i].queue[j])
				cprintf("pid : %d / state : %d / runningtime : %d / qtime : %d \n",threeQ[i].queue[j]->pid,threeQ[i].queue[j]->state,threeQ[i].queue[j]->runningtime,threeQ[i].queue[j]->qtime);
		}

	}

	cprintf("\n\n");
}
