#include <stdio.h>
#include <stdlib.h>
typedef struct polyNode {
	int coef; //계수
	int expon; //차수
	struct polyNode* next;
}polyNode;
#define TRUE	1
#define FALSE	0
void addPolynomial(polyNode** pHeadNode, int coef, int expon);
void removePolynomial(polyNode** pHeadNode, int expon);
polyNode* multiplication(polyNode* A, polyNode* B);
void showPolynomial(polyNode* pHeadNode);
void clearPolynomial(polyNode** pHeadNode);
int findPolynomial(polyNode* pHeadNode, int expon);
/* add
1. 만약 입력하려는 expon가 이미 있다면 중복을 방지하기 위해 findPolynomial함수를 이용하여 존재 여부를 파악하라
	-> 이미 존재한다면 넣을 수 없다는 문장 출력하고 main으로 돌아가자
2. 그렇지 않다면 추가하려는 노드 동적으로 할당하자
3. polynomial이 처음 상태인지 조사 -> 처음이라면 headnode로 지정해주고 종료
4. 그렇지 않다면 입력받은 expon이 들어갈 위치를 탐색
	-> 새 expon가 제일크다면 head node로 지정
	-> 탐색하면서 들어갈 자리 바로 직전에 위치한 다음 연결
*/
/* remove
1. findPolynomial를 이용해 지울 expon가 존재하는지 확인
	-> 없다면 지울 수 없다고 출력할 것
2. 지울 expon가 head node인지 확인
	-> head node라면 head node의 다음 노드를 head node로 지정
3. head node가 아니라면 지울 node의 직전 node로 이동하고 그 node와 지울 node의 다음 node와 연결
4. 지울 node 메모리 해제
*/
/* multi
1. 만약 A와 B 둘중 하나라도 NULL 이면 예외처리 진행
2. A와 B의 node를 하나씩 다 보면서 곱셈 진행해야 됨
3. 곱한 expon이 이미 C에 존재하는지 확인
	-> 있다면 존재하는 expon의 node로 들어가 coef에 계산한 coef를 더함
	-> 만약 더한 값이 0이라면 removePolynomial 함수를 이용하여 삭제
4. 곱한 expon이 다행이 C에 없다면 addPolynomial를 이용하여 C에 넣어주기
5. 곱셈이 완료된 C node를 return
*/
void addPolynomial(polyNode** pHeadNode, int coef, int expon)//다항식 A,B중 선택하여 항을 추가하는 함수
{
	int i;
	polyNode* pNewNode = NULL;
	polyNode* pTmpNode = NULL;
	
	if( findPolynomial(*pHeadNode,expon) == TRUE )
	{
		printf("this exponent already exists!\n");
		return;
	}
	
	pNewNode = (polyNode*)malloc(sizeof(polyNode));
	pNewNode->expon = expon;
	pNewNode->coef = coef;
	pNewNode->next = NULL;
	if( (*pHeadNode) == NULL )
	{
		(*pHeadNode) = pNewNode;
		(*pHeadNode)->next = NULL;
		return;
	}
	if( pNewNode->expon > (*pHeadNode)->expon )
	{
		pNewNode->next = (*pHeadNode);
		(*pHeadNode) = pNewNode;
		return;
	}
	pTmpNode = (*pHeadNode);
	while( (pNewNode->expon < pTmpNode->expon) )
	{
		if(pTmpNode->next == NULL)
		{
			pTmpNode->next = pNewNode;
			break;
		}
		if(pNewNode->expon > pTmpNode->next->expon)
		{
			pNewNode->next = pTmpNode->next;
			pTmpNode->next = pNewNode;
			break;
		}
		pTmpNode = pTmpNode->next;
	}
	
}
void removePolynomial(polyNode** pHeadNode, int expon) // A,B 중 원하는 차수를 제거하는 함수
{
	polyNode *pPrevyNode, *pDummyNode;
	
	if( findPolynomial(*pHeadNode,expon) == FALSE )
	{
		printf("there is no x^%d\n",expon);
		return;
	}
	if(expon == (*pHeadNode)->expon)
	{
		pDummyNode = (*pHeadNode); 
		(*pHeadNode) = (*pHeadNode)->next;
		free(pDummyNode);
		return;
	}
	pDummyNode = (*pHeadNode);
	while(expon != pDummyNode->expon)
	{
		pPrevyNode = pDummyNode;
		pDummyNode = pDummyNode->next;
	}
	pPrevyNode->next = pDummyNode->next;
	free(pDummyNode);
}
polyNode* multiplication(polyNode* A, polyNode* B) // A*B -> C 출력하는 함수
{
	polyNode* pTmpNodeA = A;
	polyNode* pTmpNodeB = B;
	polyNode* pTmpNodeC = NULL;
	polyNode* C = NULL;
	int coef, expon;
	
	if(A==NULL || B==NULL)
	{
		printf(" A or B is NULL!\n");
		return NULL;
	}
	while(pTmpNodeA != NULL)
	{
		while(pTmpNodeB != NULL)
		{
			coef = pTmpNodeA->coef * pTmpNodeB->coef;
			expon = pTmpNodeA->expon + pTmpNodeB->expon;
			
			addPolynomial(&pTmpNodeC,coef,expon);
			
			if( findPolynomial(C,expon) == TRUE )
			{
				polyNode* pTmpNode = C;
				while(pTmpNode != NULL)
				{
					if(pTmpNode->expon == expon)
					{
						pTmpNode->coef += coef;
						if(pTmpNode->coef == 0)
						{
							removePolynomial(&C,expon);
						}
						break;
					}
					pTmpNode = pTmpNode->next;
				}
				removePolynomial(&pTmpNodeC,expon);
			}
			pTmpNodeB= pTmpNodeB->next;
		}
	
		while(pTmpNodeC != NULL)
		{
			addPolynomial(&C,pTmpNodeC->coef,pTmpNodeC->expon);
			pTmpNodeC = pTmpNodeC->next;
		}
		pTmpNodeA = pTmpNodeA->next;
		pTmpNodeB = B;
	}
	return C;
}
void showPolynomial(polyNode* pHeadNode) // 현재의 A,B 상태를 출력하는 함수
{
	polyNode* pTmpNode;
	printf("\n");
	pTmpNode = pHeadNode;
	if(pHeadNode == NULL)
	{
		printf("\n===================\n");
		return;
	}
	while(pTmpNode != NULL)
	{
		printf("%d*X^%d ",pTmpNode->coef , pTmpNode->expon);
		if(pTmpNode->next != NULL)
			printf("+ ");
		pTmpNode = pTmpNode->next;
	}
	printf("\n===================\n");
}
void clearPolynomial(polyNode** pHeadNode)  // A,B 중 선택해 초기화
{
	polyNode* pTmpNode=(*pHeadNode);
	polyNode* pDummyNode=NULL;
	if( (*pHeadNode) == NULL )
	{
		printf("already clear!\n");
		return;
	}
	while(pTmpNode != NULL)
	{
		pDummyNode = pTmpNode;
		pTmpNode = pTmpNode->next;
		free(pDummyNode);
	}
	(*pHeadNode)=NULL;
}
int findPolynomial(polyNode* pHeadNode, int expon) // A,B 중 선택해 원하는 차수가 존재하는지 확인
{
	polyNode* pTmpNode = pHeadNode;
	if(pHeadNode == NULL)
		return FALSE;
	while(pTmpNode != NULL)
	{
		if(pTmpNode->expon == expon)
		{
			return TRUE;
		}
		pTmpNode = pTmpNode->next;
	}
	return FALSE;
}
int main()
{
	int mode, coef, expon;
	char poly;
	
	polyNode *A, *B, *C;
	A = B = C = NULL;
	do
	{
		printf("\nSelect the mode\n");
		printf("=================\n");
		printf("1: add polynomial\n");
		printf("2: remove polynomial\n");
		printf("3: Multiplication\n");
		printf("4: show polynomial\n");
		printf("5: clear polynomial\n");
		printf("-1: exit the program\n");
		printf("==================\n");
		printf("mode: ");
		scanf("%d", &mode);
		switch (mode)
		{
		case 1:
			printf("\nChoose the polynomial to add ('A'or'B')\n");
			printf("Which polynomial?: ");
			scanf(" %c", &poly);
			if (poly != 'A' && poly != 'B')
				printf("Please choose right polynomial (A or B)\n");
			else
			{
				//계수와 차수를 입력 받아서 polynomial에 추가하는 함수 실행
				//이미 존재하는 차수를 입력 받을 경우 추가할 수 없음
				printf("input the coef: ");
				scanf("%d", &coef);
				printf("input the expon: ");
				scanf("%d", &expon);
				if (poly == 'A')
					addPolynomial(&A, coef, expon);
				else
					addPolynomial(&B, coef, expon);
			}
			break;
		case 2:
			printf("\nChoose the polynomial to remove ('A'or'B')\n");
			printf("Which polynomial?: ");
			scanf(" %c", &poly);
			if (poly != 'A' && poly != 'B')
				printf("Please choose right polynomial (A or B)\n");
			else
			{
				printf("input the expon: ");
				scanf("%d", &expon);
				//차수를 입력 받아서 polynomial에서 제거하는 함수 실행
				if (poly == 'A')
					removePolynomial(&A, expon);
				else
					removePolynomial(&B, expon);
			}
			break;
		case 3:
			printf("\nMultiplication with A and B\n");
			//C = A * B 연산을 수행
			C = multiplication(A, B);
			printf("C: ");
			//C의 결과를 출력
			showPolynomial(C);
			break;
		case 4:
			//A,B polynomial 각각 출력
			printf("A: ");
			showPolynomial(A);
			printf("B: ");
			showPolynomial(B);
			break;
		case 5:
			printf("\nChoose the polynomial to clear ('A'or'B')\n");
			printf("Which polynomial?: ");
			scanf(" %c", &poly);
			if (poly != 'A' && poly != 'B')
				printf("Please choose right polynomial (A or B)\n");
			else
				//해당 polynomial를 초기화 시키는 함수 실행
			{
				if (poly == 'A')
					clearPolynomial(&A);
				else
					clearPolynomial(&B);
			}
			break;
		default:
			break;
		}
	} while (mode != -1);
	return 0;
}