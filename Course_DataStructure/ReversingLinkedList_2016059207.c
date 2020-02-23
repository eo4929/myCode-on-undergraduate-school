#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

typedef struct Node {
	int data;
	struct Node* nextNode;
}Node;
// 주의! LinkedList는 노드가 아니다! headNode.nextNode부터가 0번째 노드임
typedef struct LinkedList {
	int curCount;
	Node headNode;
}LinkedList;


typedef struct StackNode{
	int data;
	struct StackNode* next;
}StackNode;


int addNode(LinkedList* pList, int pos, int data);
int removeNode(LinkedList* pList, int pos);
void showNode(LinkedList* pList);
int isEmpty(LinkedList* pList);
int findPos(LinkedList* pList, int data);
void makeEmpty(LinkedList* pList);


void pushLinkedStack(StackNode** top, int data);
StackNode* popLinkedStack(StackNode** top);
void showLinkedStack(StackNode* top);
StackNode* topLinkedStack(StackNode* top);
void deleteLinkedStack(StackNode** top);
int stackIsEmpty(StackNode* top);


void reverseList(LinkedList* pList, StackNode** top);

int main()
{
	int pos;
	LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
	linkedList->curCount = 0;
	linkedList->headNode.nextNode = NULL;

	StackNode* top = NULL;
	StackNode* pNode;
	
	
	//showNode(linkedList);
	addNode(linkedList,0,10);
	addNode(linkedList,5,100);
	addNode(linkedList,1,20);
	addNode(linkedList,2,30);
	addNode(linkedList,1,50);
	addNode(linkedList,1,70);
	addNode(linkedList,1,40);

	showNode(linkedList);

	reverseList(linkedList,&top);

	showNode(linkedList);

	/*
	removeNode(linkedList, 1);
	showNode(linkedList);

	pos = findPos(linkedList, 30);
	printf("the location of node with data '30' : %d \n", pos);
	*/

	makeEmpty(linkedList);
	showNode(linkedList);
	
	
	return 0;
}

void reverseList(LinkedList* pList, StackNode** top)
{
	Node *pNode = NULL;
	StackNode *sNode = NULL;

	printf("Reverse Linked List!\n");
	//Stack에 List 저장
	pNode = pList->headNode.nextNode;
	
	while(pNode != NULL)
	{
		pushLinkedStack(top,pNode->data);
		pNode = pNode->nextNode;
	}
	makeEmpty(pList);
	
	//List에 Stack 저장
	while(!stackIsEmpty(*top))
	{
		sNode = popLinkedStack(top);
		addNode(pList,pList->curCount,sNode->data);
	}
	
}

// 여기서부터 리스트 함수
int addNode(LinkedList* pList, int pos, int data)
{
	int i = 0;
	Node *pNewNode = NULL, *pTempNode = NULL; // pTempNode는 이전노드를 가리키기 위한 용도

	if(pList == NULL)
	{
		printf("addNode() error1 \n");
		return FALSE;
	}
	if(pos < 0 || pos > pList->curCount)
	{
		printf("addNode() error2 : 추가 범위 초과 \n");
		return FALSE;
	}

	pNewNode = (Node*)malloc(sizeof(Node));
	if(!pNewNode)
	{
		printf("addNode() error3 \n");
		return FALSE;
	}

	pNewNode->data = data;
	pNewNode->nextNode = NULL;


	pTempNode = &(pList->headNode);
	for(i=0 ; i<pos; i++) //  직전노드까지 이동
		pTempNode = pTempNode->nextNode;
	
	
	pNewNode->nextNode = pTempNode->nextNode;
	pTempNode->nextNode = pNewNode;
	
	pList->curCount++;

	return TRUE;
}


int removeNode(LinkedList* pList, int pos)
{
	int i =0;
	Node *pDelNode = NULL, *pTempNode = NULL;

	if(pList == NULL)
	{
		printf("removeNode() error1\n");
		return FALSE;
	}

	if(pos <0 || pos > pList->curCount)
	{
		printf("removeNode() error2 : 삭제 범위 초과\n");
		return FALSE;
	}

	pTempNode = &(pList->headNode);
	for(i=0 ; i<pos; i++) //  직전노드까지 이동
		pTempNode = pTempNode->nextNode;

	pDelNode = pTempNode->nextNode;
	pTempNode->nextNode = pDelNode->nextNode;

	pList->curCount--;

	free(pDelNode);
	return TRUE;
}


void showNode(LinkedList* pList)
{
	Node *pNode = NULL;

	if(pList == NULL)
	{
		printf("showNode() error\n");
		return;
	}

	printf("현재 Node 개수 : %d \n", pList->curCount);
	pNode = pList->headNode.nextNode;

	while(pNode != NULL )
	{
		printf("[%d]\n",pNode->data);
		pNode = pNode->nextNode;
	}

	printf("----------------\n");
}


int isEmpty(LinkedList* pList)
{
	if(pList == NULL)
	{
		printf("isEmpty() error\n");
		return -1;
	}

	if( pList->headNode.nextNode == NULL)
		return TRUE;
	else
		return FALSE;
}

int findPos(LinkedList* pList, int data)
{
	int pos = 0;
	Node *pNode = NULL;

	if(pList == NULL)
	{
		printf("findPos() error \n");
		return FALSE;
	}

	pNode = pList->headNode.nextNode;

	while(pNode != NULL)
	{
		if(pNode->data == data)
		{
			return pos;
		}
		else
		{
			pNode = pNode->nextNode;
			pos++;
		}
		
	}

	return FALSE;
}

void makeEmpty(LinkedList* pList)
{
	Node* pDummyNode = NULL, *pTmpNode = NULL;

	if(pList != NULL)
	{
		pTmpNode = pList->headNode.nextNode;
		
		while(pTmpNode != NULL)
		{
			pDummyNode = pTmpNode;
			pTmpNode = pTmpNode->nextNode;

			free(pDummyNode);
		}

		pList->headNode.nextNode = NULL;
		pList->curCount = 0;
	}
}


// 여기서부터 스택 함수
void pushLinkedStack(StackNode** top, int data)
{
	StackNode *pNode = NULL;

	pNode = (StackNode*)malloc(sizeof(StackNode));
	pNode->data = data;
	pNode->next = NULL;

	if (stackIsEmpty(*top))
	{
		*top = pNode;
		//printf("test1\n"); //test code
	}
	else
	{
		pNode->next = *top;
		*top = pNode;
		//printf("test2\n"); //test code
	}
}
StackNode* popLinkedStack(StackNode** top)
{
	StackNode *pNode = NULL;

	if(stackIsEmpty(*top))
	{
		printf("Stack is empty\n");
		return pNode;
	}

	pNode = *top;
	*top = (*top)->next;

	return pNode;
}
void showLinkedStack(StackNode* top)
{
	StackNode *pNode = NULL;
	if(stackIsEmpty(top))
	{
		printf("the Stack is empty\n");
		return;
	}

	pNode = top;
	printf("========= Show Stack ==========\n");
	while(pNode != NULL)
	{
		printf("[%d]\n",pNode->data);
		pNode = pNode->next;
	}

	printf("======================\n");
}
StackNode* topLinkedStack(StackNode* top)
{
	StackNode *pNode = NULL;

	if(! stackIsEmpty(top) )
		pNode = top;

	return pNode;
}
void deleteLinkedStack(StackNode** top)
{
	StackNode* pNode = NULL, *pDelNode = NULL;
	pNode = *top;

	while(pNode != NULL)
	{
		pDelNode = pNode;
		pNode = pNode->next;

		free(pDelNode);
	}

	*top = NULL;
}

int stackIsEmpty(StackNode* top)
{
	if(top == NULL)
		return TRUE;
	else
		return FALSE;
}
