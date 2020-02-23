#include <stdio.h>
#include <stdlib.h>

#define M_WAY 3

typedef struct BTNode {
	int n;
	int isLeaf;
	int isRoot;
	int keys[M_WAY];

	struct BTNode* childs[M_WAY + 1];
}BTNode;

BTNode* initBTNode();
BTNode* BTInsert(BTNode* root, int key);
BTNode* splitChild(BTNode* root);
void inorderTraversal(BTNode* root);

BTNode* initBTNode()
{
	int i;
	BTNode* newBTNode;
	newBTNode = (BTNode*)malloc(sizeof(BTNode));
	newBTNode->n = 0;
	newBTNode->isLeaf = 1;
	newBTNode->isRoot = 0;

	for (i = 0; i < M_WAY + 1; i++)
		newBTNode->childs[i] = NULL;

	return newBTNode;
}
BTNode* BTInsert(BTNode* root, int key)
{
	int i, pos, mid;
	BTNode* split;

	if (root->isLeaf)
	{
		for (pos = 0; pos < root->n; pos++)
			if (root->keys[pos] > key)
				break;

		for (i = root->n; i > pos; i--)
			root->keys[i] = root->keys[i - 1];
		root->keys[pos] = key;
		root->n++;

		if (root->n == M_WAY && root->isRoot)
		{
			return splitChild(root);
		}

		return root;
	}
	else
	{
		for (pos = 0; pos < root->n; pos++) // pos에 위치 정보 저장
			if (root->keys[pos] > key)
				break;

		root->childs[pos] = BTInsert(root->childs[pos], key);

		if ((root->childs[pos])->n == M_WAY)
		{
			split = splitChild(root->childs[pos]);

			for (i = root->n; i > pos; i--)
			{
				root->keys[i] = root->keys[i - 1];
				root->childs[i+1] = root->childs[i];
			}
			root->keys[pos] = split->keys[0];
			root->n++;
			root->childs[pos] = split->childs[0];
			root->childs[pos+1] = split->childs[1];
		}

		if (root->n == M_WAY && root->isRoot)
			return splitChild(root);
		return root;
	}
}

BTNode* splitChild(BTNode* root)
{
	int i, mid;

	BTNode* newParent;
	BTNode* newSibling;

	newParent = initBTNode();
	newParent->isLeaf = 0;

	if (root->isRoot)
		newParent->isRoot = 1;
	root->isRoot = 0;

	newSibling = initBTNode();
	newSibling->isLeaf = root->isLeaf;

	mid = (M_WAY - 1) / 2;
	for (i = mid + 1; i < M_WAY; i++)
	{
		newSibling->childs[i - (mid + 1)] = root->childs[i];
		newSibling->keys[i - (mid + 1)] = root->keys[i];
		newSibling->n++;

		root->childs[i] = NULL;
		root->keys[i] = 0;
		root->n--;
	}

	newSibling->childs[i - (mid + 1)] = root->childs[i];

	root->childs[i] = NULL;

	newParent->keys[0] = root->keys[mid];
	newParent->n++;

	root->keys[mid] = 0;
	root->n--;

	newParent->childs[0] = root;
	newParent->childs[1] = newSibling;

	return newParent;
}

void inorderTraversal(BTNode* root)
{
	int i;
	printf("\n");
	for (i = 0; i < root->n; i++)
	{
		if (!(root->isLeaf))
		{
			inorderTraversal(root->childs[i]);
			printf("   ");
		}

		printf("%d", root->keys[i]);
	}

	if (!(root->isLeaf))
	{
		inorderTraversal(root->childs[i]);
	}
	printf("\n");
}

int main()
{
	BTNode* root;
	int i, n, t;

	root = initBTNode();
	root->isRoot = 1;

	printf("넣을 데이터의 개수 : ");
	scanf("%d", &n);

	for (i = 0; i < n; i++)
	{
		printf("데이터를 입력하세요 : ");
		scanf("%d", &t);
		root = BTInsert(root, t);
	}
	printf("트리 출력 \n");
	inorderTraversal(root);

	return 0;
}