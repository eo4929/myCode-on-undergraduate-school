#include <stdio.h>
#include <stdlib.h>

/*
			4
	2				6
1		3		5		7
*/
typedef struct Node
{
	int data;
	struct Node* leftChild;
	struct Node* rightChild;
}Node;

void insertTreeNode(Node** p, int value);
void printTreePreorder(Node* pNode);
void printTreeInorder(Node* pNode);
void printTreePostorder(Node* pNode);

Node* searchTreeNode(Node* p, int value);
Node* searchTreeParentNode(Node* p, int value);
void deleteTreeNode(Node** p, int value);

void copyTreeNode(Node* src, Node** dst);
int compareTwoTree(Node* t1, Node* t2);

int main()
{
	Node* pParentNode = NULL;
	Node* cParentNode = NULL;

	//insert data into pParentNode
	insertTreeNode(&pParentNode, 4);
	insertTreeNode(&pParentNode, 2);
	insertTreeNode(&pParentNode, 6);
	insertTreeNode(&pParentNode, 1);
	insertTreeNode(&pParentNode, 3);
	insertTreeNode(&pParentNode, 5);
	insertTreeNode(&pParentNode, 7);

	//print data in pParentNode
	printf("Tree(pParentNode)\n");
	printf("Preorder\t : ");
	printTreePreorder(pParentNode);
	printf("\n");
	printf("Inorder\t\t : ");
	printTreeInorder(pParentNode);
	printf("\n");
	printf("Postorder\t : ");
	printTreePostorder(pParentNode);
	printf("\n\n");

	// search data in pParentNode
	printf("Search Data 3\n");
	if (searchTreeNode(pParentNode, 3) != NULL)
		printf("3 is existed\n\n");
	else
		printf("3 is not existed\n\n");
	
	// delete data in pParentNode
	
	//printf("Delete data 3\n");
	//deleteTreeNode(&pParentNode, 3);
	//printf("Delete data 2\n");
	//deleteTreeNode(&pParentNode, 2);
	printf("Delete data 4\n");
	deleteTreeNode(&pParentNode, 4);
	
	

	// search data in pParentNode
	printf("Search Data 4\n");
	if (searchTreeNode(pParentNode, 4) != NULL)
		printf("4 is existed\n\n");
	else
		printf("4 is not existed\n\n");

	//print data in pParentNode
	printf("Tree(pParentNode)\n");
	printf("Preorder\t : ");
	printTreePreorder(pParentNode);
	printf("\n");
	printf("Inorder\t\t : ");
	printTreeInorder(pParentNode);
	printf("\n");
	printf("Postorder\t : ");
	printTreePostorder(pParentNode);
	printf("\n\n");

	//Copy Tree(pParentNode) to Tree(cParentNode)
	copyTreeNode(pParentNode, &cParentNode);

	//print data in cParentNode
	printf("Preorder\t : ");
	printTreePreorder(cParentNode);
	printf("\n");
	printf("Inorder\t\t : ");
	printTreeInorder(cParentNode);
	printf("\n");
	printf("Postorder\t : ");
	printTreePostorder(cParentNode);
	printf("\n\n");

	//Compare Data 
	if (compareTwoTree(pParentNode, cParentNode))
		printf("Two trees are equals\n");
	else
		printf("Two trees are different\n");


	return 0;
}

void insertTreeNode(Node** p, int value)
{
	if ((*p) == NULL)
	{
		*p = (Node*)malloc(sizeof(Node));
		(*p)->data = value;
		(*p)->leftChild = NULL;
		(*p)->rightChild = NULL;
	}
	else if ((*p)->data > value)
	{
		//이부분 틀림 이중포인터 주의;;
		insertTreeNode((&(*p)->leftChild), value);
	}
	else
	{
		insertTreeNode((&(*p)->rightChild), value);
	}
}

void printTreePreorder(Node* pNode)
{
	if (pNode == NULL)
		return;

	printf("%3d", pNode->data);
	printTreePreorder(pNode->leftChild);
	printTreePreorder(pNode->rightChild);
}
void printTreeInorder(Node* pNode)
{
	if (pNode == NULL)
		return;

	printTreeInorder(pNode->leftChild);
	printf("%3d", pNode->data);
	printTreeInorder(pNode->rightChild);
}
void printTreePostorder(Node* pNode)
{
	if (pNode == NULL)
		return;

	printTreePostorder(pNode->leftChild);
	printTreePostorder(pNode->rightChild);
	printf("%3d", pNode->data);
}

Node* searchTreeNode(Node* p, int value)
{
	// p가 NULL인지 확인해야 됨
	while (p!=NULL && p->data != value)
	{
		if (p->data < value)
		{
			p = p->rightChild;
		}
		else if(p->data > value)
		{
			p = p->leftChild;
		}
	}

	return p;
}
Node* searchTreeParentNode(Node* p, int value)
{
	Node* parentNode = NULL;

	while (p != NULL && p->data != value)
	{
		parentNode = p;

		if (p->data < value)
		{
			p = p->rightChild;

		}
		else if (p->data > value)
		{
			p = p->leftChild;
		}
	}

	return parentNode;
}
void deleteTreeNode(Node** p, int value)
{
	Node* parentNode;
	Node* delNode;
	Node* childNode;
	Node* succNode;

	// 이부분 틀림 p는 이중포인터였음;;
	parentNode = searchTreeParentNode(*p,value);
	delNode = searchTreeNode(*p,value);
	if (delNode == NULL)
	{
		printf("There is no %d\n", value);
		return;
	}

	// case1
	if ( (delNode->leftChild == NULL) 
		&& (delNode->rightChild == NULL))
	{
		//지울노드가 root인 경우
		if (parentNode == NULL)
		{
			(*p) = NULL;
		}
		else
		{
			if (delNode == parentNode->leftChild)
			{
				parentNode->leftChild = NULL;
			}
			else if (delNode == parentNode->rightChild)
			{
				parentNode->rightChild = NULL;
			}
		}
	}

	//case2
	else if ((delNode->leftChild == NULL)
		|| (delNode->rightChild == NULL))
	{
		childNode = (delNode->rightChild != NULL)
			? delNode->rightChild
			: delNode->leftChild;

		if (parentNode == NULL)
		{
			(*p) = childNode;
		}
		else
		{
			//이부분 주의! 실수함
			if (parentNode->leftChild == delNode)
			{
				parentNode->leftChild = childNode;
			}
			else
			{
				parentNode->rightChild = childNode;
			}
		}
	}
	
	// case 3
	else
	{
		succNode = delNode->rightChild;
		childNode = delNode; // suc의 부모노드
		while(succNode->leftChild != NULL)
		{
			childNode = succNode; // suc의 부모노드
			succNode = succNode->leftChild;
		}
		if (succNode->rightChild != NULL)
		{
			childNode->leftChild = succNode->rightChild;
		}
		else
		{
			childNode->leftChild = NULL;
		}
		delNode->data = succNode->data;
		delNode = succNode;
	}
	free(delNode);
}

void copyTreeNode(Node* src, Node** dst)
{
	if (src == NULL)
		return;
	insertTreeNode(dst, src->data);
	copyTreeNode(src->leftChild, dst);
	copyTreeNode(src->rightChild, dst);
}
int compareTwoTree(Node* t1, Node* t2)
{
	if (t1 == NULL && t2 == NULL)
		return 1;

	if (t1 == NULL || t2 == NULL || t1->data != t2->data)
		return 0;

	compareTwoTree(t1->leftChild, t2->leftChild);
	compareTwoTree(t1->rightChild, t2->rightChild);
}