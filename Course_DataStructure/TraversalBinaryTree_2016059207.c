#include <stdio.h>
#include <stdlib.h>

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

int main()
{
	Node* pParentNode = NULL;

	insertTreeNode(&pParentNode, 4);
	insertTreeNode(&pParentNode, 2);
	insertTreeNode(&pParentNode, 6);
	insertTreeNode(&pParentNode, 1);
	insertTreeNode(&pParentNode, 3);
	insertTreeNode(&pParentNode, 5);
	insertTreeNode(&pParentNode, 7);

	printf("Preorder : \n");
	printTreePreorder(pParentNode);
	printf("\n");
	printf("Inorder : \n");
	printTreeInorder(pParentNode);
	printf("\n");
	printf("Postorder : \n");
	printTreePostorder(pParentNode);

	printf("\n");
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