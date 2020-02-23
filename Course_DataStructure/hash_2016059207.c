#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 10
//#define TABLE_SIZE 5 //실습용
#define TABLE_SIZE 13 // 과제용
#define empty(e) (strlen(e.key)==0) // 과제용, 비어있으면 1을 리턴할 것임
#define equal(e1,e2) (!strcmp(e1.key,e2.key)) //실습용, 과제용 , 같으면 1이 나오게 할려고 !붙인 것이다

typedef struct Element
{
	char key[KEY_SIZE]; // key 값이 문자열, 최대크기는 10
}Element;

/*
typedef struct ListNode
{
Element item;
struct ListNode *link;
}ListNode;
*/


//ListNode *hashTable[TABLE_SIZE]; //실습용
Element hashTable[TABLE_SIZE];

/* //실습용
void initTable(ListNode* ht[])
{
int i;
for (i = 0; i < TABLE_SIZE; i++)
ht[i] = NULL;
}
*/

void initTable(Element ht[])
{
	int i;
	for (i = 0; i < TABLE_SIZE; i++)
		ht[i].key[0] = NULL;
}

int transform(char* key)
{
	int i;
	int number = 0;
	int size = strlen(key);
	for (i = 0; i < size; i++)
		number = number + key[i];

	return number;
}

int hashFunction(char *key)
{
	return transform(key) % TABLE_SIZE;
}

//과제용
void addHashTable(Element item, Element ht[])
{
	int i, hashValue;
	i = hashValue = hashFunction(item.key);

	
	while(!empty(ht[i]))
	{
		if (equal(item, ht[i]))
		{
			printf("중복 삽입 에러 발생!\n");
			return;
		}

		i = (i + 1) % TABLE_SIZE;

		if (i == hashValue)
		{
			printf("버킷이 모두 꽉찼습니다!\n");
			return;
		}
	}

	ht[i] = item;
}

/* //실습용
void addHashTable(Element item, ListNode* ht[])
{
int hashValue = hashFunction(item.key); // 해쉬값 구하기
ListNode *ptr; // 새로운 노드 동적할당용 포인터
ListNode *node = ht[hashValue]; // 해시테이블의 버킷의 시작포인트

for (; node; node = node->link) // 중복인지 아닌지 테스트
{
//equal사용 -> 같은게 있으면 중복이라고 하고 끝내기
if (equal(item, node->item)) // 주의! 그냥 element를 넣어주는것이다
{
printf("중복 삽입 에러 발생!\n");
return;
}
}

ptr = (ListNode*)malloc(sizeof(ListNode));
ptr->item = item;

ptr->link = ht[hashValue]; // 주의! node를 넣으면 안된다!!
ht[hashValue] = ptr;
}
*/

//과제용
void hashSearch(Element item, Element ht[])
{
	int i, hashValue;
	i = hashValue = hashFunction(item.key);

	while (!empty(ht[i]))
	{
		if (equal(item, ht[i]))
		{
			printf("탐색 성공 : 존재합니다.\n");
			return;
		}

		i = (i + 1) % TABLE_SIZE;

		if (i == hashValue)
		{
			printf("버킷이 모두 꽉찼습니다!\n");
			return;
		}
	}

	printf("테이블에 찾는 값이 없습니다.\n");
}

/* // 실습용
void hashSearch(Element item, ListNode* ht[])
{
ListNode *node;

int hashValue = hashFunction(item.key);
for (node = ht[hashValue]; node; node = node->link)
{
if (equal(node->item, item))
{
printf("탐색 성공 : 존재합니다.\n");
return;
}
}

printf("탐색 실패\n");
}
*/

/* // 실습용
void printHashTable(ListNode* ht[])
{
int i;
ListNode *temp;
for (i = 0; i < TABLE_SIZE; i++)
{
temp = ht[i];
for(; temp; temp = temp->link)
{
printf("%s\t", temp->item.key);
//temp = temp->link; 실수주의!
}
printf("\n");
}
}
*/

//과제용
void printHashTable(Element ht[])
{
	int i;
	for (i = 0; i < TABLE_SIZE; i++)
	{
		printf("[%d]       %s\n", i, ht[i].key);
	}
}

int main()
{
	Element temp;
	int op;

	while (1)
	{
		printf("연산 입력(0 : 추가, 1 : 탐색, 2 : 종료) = ");
		scanf("%d", &op);

		if (op == 2) break;

		printf("키 입력 : ");
		scanf("%s", temp.key);

		if (op == 0)
			addHashTable(temp, hashTable);
		else if (op == 1)
			hashSearch(temp, hashTable);
		
		printHashTable(hashTable);
	}

	return 0;
}

/* //실습용
int main()
{
Element temp;
int op;

while (1)
{
printf("연산 입력(0 : 추가, 1 : 탐색, 2 : 출력, 3 : 종료) = ");
scanf("%d", &op);

if (op == 3)
break;
if (op == 2)
printHashTable(hashTable);
else
{
printf("키 입력 : ");
scanf("%s", temp.key);

if (op == 0)
addHashTable(temp, hashTable);
else if (op == 1)
hashSearch(temp, hashTable);
}
}

return 0;
}
*/
