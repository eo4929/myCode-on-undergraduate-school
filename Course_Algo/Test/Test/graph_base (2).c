/*
	Graph를 다루기 위해 사용할 코드 파일이에요.
	(9주차때 직접 만든 요소들에 약간의 기능을 덧대어 두었어요)

	주:
		- 여러 algorithm들을 보다 편리하게 구현할 수 있도록 작성 / 편집 순서에 따라 코드를 나열해 두었어요.
		- 작성 편의를 위해 typedef을 조금 넣어 두었어요.

	변경 이력:

	2018. 7. 9.
		- V *Get_V(G *g, int idx_v), E *Get_E(G *g, int idx_v_from, int idx_v_to)를 추가했어요.
			> 해당되는 vertex / edge가 graph에 존재하는 경우 해당 object에 대한 포인터 값을 return해요.
			> 해당되는 vertex / edge가 graph에 없는 경우 null 포인터 값을 return해요.
			> Adjacency list를 쓴다면 Get_E()가 나름 쓰일 수 있을 듯 해요.
			  Adjacency matrix를 쓸 때는 그냥 g->e[i][j] 하면 될 듯.
			  Get_V()도 사실상 그냥 g->v[i] 하면 되긴 해요(대칭성을 유지하기 위해 만들어 둠).

		- G *Load_Graph(FILE *fp, int isUndirectedGraph)를 추가했어요.
			> 파일(키보드 포함)에서 숫자들을 읽어 새 graph를 만들어 return하는 함수예요.
			> 현재 숫자 읽는 순서는...
				~ 첫 번째 숫자: Vertex 수
				~ 두 번째 숫자: Edge 수
				~ 이후로 숫자를 두 개씩 끊어 읽어 가며 edge를 만들어 추가해요.
				  isUndirectedGraph 값이 nonzero면 양방향 하나씩,
				  값이 zero면 앞 idx -> 뒤 idx 방향 하나를 만들어요.
			> 문제에 따라 입력 방식이 조금씩 다를테니 그 때 그 때 조금씩 수정해 쓰면 될 듯!

		- void Print_Graph(FILE *fp, G *g)를 추가했어요.
			> 파일(콘솔 창 포함)에 해당 graph와 관련한 정보를 print해요.
			  초기 버전은 단순 디버그 용도로 쓸 수 있도록 구성해 두었어요.

			> 문제에 따라 출력 방식은 매우 많이 다르니 사실상 출력 기능은 main()에서 짜게 될 듯 해요.

*/


#include <stdio.h>
#include <stdlib.h>

/*
	Vertex 설정
*/

// TODO: 문제의 특징에 따라 각 vertex마다 어떤 값을 담을 지 파악하여 적절한 멤버 선언 적기
#define V_WHITE 0
#define V_GRAY  1
#define V_BLACK 2

struct Vertex_t
{
	int state;
	int distance;
	int idx_v_from;
};

typedef struct Vertex_t V;

// TODO: Vertex 구조체의 구성에 따라 적절한 값을 미리 담아 두도록 구성하기
void Init_Vertex(V *vertexToInitialize)
{
	vertexToInitialize->state = V_WHITE;
	vertexToInitialize->distance = 0x7FFFFFFF;
	vertexToInitialize->idx_v_from = -1;
}


/*
	Edge 설정
*/

// TODO: 문제의 특징에 따라 각 edge마다 어떤 값을 담을 지 파악하여 구조체 형식으로 만들거나 그냥 int 형식 쓰기
//       (적어도 'edge 있음' 정도는 나타내야 하므로 int 값 하나 정도는 필요할 거예요)
//#define USE_STRUCTURED_EDGE

#ifdef USE_STRUCTURED_EDGE
struct Edge_t
{
	int weight;
};

typedef struct Edge_t E;
#else

typedef int E;

#endif

// TODO: Edge 구성에 따라 적절한 값을 미리 담아 두도록 구성하기
void Init_Edge(E *edgeToInitialize)
{
	*edgeToInitialize = 0;
}





/*
	Graph 설정

	주:
		- Graph를 구성하는 vertex 수는 늘 고정되어 있다고 간주하고 있어요. (처음 만들 때 정함)
		- Edge 또한 추가만 하고 제거는 안 할 것이라고 간주하고 있어요.
			> 중복된 edge 추가에 대해서는 특별히 검사하지 않을 예정이에요.
		- 원본 graph가 Undirected graph일 때 edge 하나에 대한 정보는 두 개 생겨요. (포인터 형식을 써서 서로 '공유'하게 만들지 않음)
			> Vertex에 담긴 값은 종종 변경되지만, edge에 담긴 값을 변경하는 경우는 별로 없어요.
		- 모든 vertex는 0번부터 순서대로 번호를 갖는다고 간주하고 있어요.
*/

#define USE_ADJACENCY_LIST

#ifdef USE_ADJACENCY_LIST
// Adjacency list를 구성할 때만 사용할 node 구조체예요.
struct E_node_t
{
	E e; 
	int idx_v_to; 
	struct E_node_t *next; 
};

typedef struct E_node_t E_node;
#endif

struct Graph_t
{
	int n_V;

	V *v;

#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *e;
#else
	// Adjacency matrix
	E **e;
#endif
};

typedef struct Graph_t G;



/*
	Graph 자체에 대한 manipulation을 위한 함수들
*/

// 주어진 수만큼의 vertex를 갖는 새 graph를 만듭니다.
// (Edge는 나중에 Add_Edge() 써서 추가해야 함!)
G *New_Graph(int n_V)
{
	int idx;
	int idx_column;

	// Graph 본체 할당
	G *result = malloc(sizeof(G));

	// V 할당
	result->n_V = n_V;
	result->v = malloc(sizeof(V) * n_V);

	for ( idx = 0; idx < n_V; ++idx )
		Init_Vertex(&result->v[idx]);

	// E 할당
#ifdef USE_ADJACENCY_LIST
	// Adjacency list

	// 각 vertex별 list의 head들을 담을 배열 할당
	result->e = malloc(sizeof(E_node) * n_V);

	// 각 head들을 null 포인터로 초기화
	for ( idx = 0; idx < n_V; ++idx )
		result->e[idx].next = 0;
#else
	// Adjacency matrix

	// Matrix 할당
	// 첫 4 * n_V byte는 row vector용, 나머지는 n_V개 짜리 column array들
	result->e = malloc(sizeof(E*) * n_V + sizeof(E) * n_V * n_V);

	for ( idx = 0; idx < n_V; ++idx )
	{
		// row pointer 지정(우항이 복잡해보여도 실질적으로는 그냥 대괄호 연산 두 번 진행함)
		result->e[idx] = &((E*)&result->e[n_V])[idx * n_V];

		for ( idx_column = 0; idx_column < n_V; ++idx_column )
			Init_Edge(&result->e[idx][idx_column]);
	}
#endif

	return result;
}

// 사용이 끝난 graph를 해제합니다.
void Delete_Graph(G *g)
{
	// E 해제
#ifdef USE_ADJACENCY_LIST
	// Adjacency list

	int idx_v;
	E_node *ptr;
	E_node *ptr_next;

	// 각 list들 해제
	for ( idx_v = 0; idx_v < g->n_V; ++idx_v )
		for ( ptr = g->e[idx_v].next; ptr != 0; )
		{
			ptr_next = ptr->next;
			free(ptr);
			ptr = ptr_next;
		}

	// list 배열 해제
	free(g->e);

#else
	// Adjacency matrix

	// 통짜로 malloc()해 놨으니 그냥 해제하면 됨
	free(g->e);
#endif

	// V 해제
	free(g->v);

	// Graph 해제
	free(g);
}

// Graph에 새 directed edge를 추가합니다.
// 원본 graph가 undirected graph라면 그냥 이거 두 번 호출하면 될 듯 해요.
void Add_Edge(G *g, int idx_v_from, int idx_v_to)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *newNode;
	E_node *ptr_nodeToAdd;

	// 새 node 만들고 정보 담기
	newNode = malloc(sizeof(E_node));
	newNode->next = 0;
	newNode->idx_v_to = idx_v_to;
	Init_Edge(&newNode->e);

	// 이걸 엇다 담을 지 정하기(시간이 좀 더 걸리긴 하지만 입력받은 순서대로 나열되도록 늘 맨 뒤에 담고 있어요)
	for ( ptr_nodeToAdd = &g->e[idx_v_from]; ptr_nodeToAdd->next != 0; ptr_nodeToAdd = ptr_nodeToAdd->next )
		;

	// 거기다 담기
	ptr_nodeToAdd->next = newNode;
#else
	// Adjacency matrix
	g->e[idx_v_from][idx_v_to] = 1;
#endif
}


// v[from] -> v[to] edge가 존재하는지 여부를 return합니다.
int Adjacent(G *g, int idx_v_from, int idx_v_to)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *ptr;

	for ( ptr = g->e[idx_v_from].next; ptr != 0; ptr = ptr->next )
		if ( ptr->idx_v_to == idx_v_to )
			return 1;

	return 0;
#else
	// Adjacency matrix

	return g->e[idx_v_from][idx_v_to] != 0;
#endif
}

// v[from] -> v[to] edge가 존재하는 모든 v[to]들의 수를 return합니다.
// 보통은 아래에 있는 Neighbors()를 호출하기 전에 적절한 메모리 칸을 잡아 두기 위해 호출될 거예요.
int Count_Neighbors(G *g, int idx_v_source)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *ptr;
	int count = 0;

	for ( ptr = g->e[idx_v_source].next; ptr != 0; ptr = ptr->next )
		++count;

	return count;
#else
	// Adjacency matrix
	int idx_v_target;
	int count = 0;

	for ( idx_v_target = 0; idx_v_target < g->n_V; ++idx_v_target )
		if ( g->e[idx_v_source][idx_v_target] != 0 )
			++count;

	return count;
#endif
}

// v[from] -> v[to] edge가 존재하는 모든 v[to]들의 'index'를 out_idxs_v_target 값으로 특정되는 배열에 순서대로 담고, 그 수를 return합니다.
// 호출자가 위에 정의한 Count_Neighbors()를 먼저 호출해서 적절한 크기를 잡아 두거나,
// 귀찮으면 그냥 V칸짜리 하나 잡아 놓고 재활용해도 될 듯 해요(담은 횟수는 return값을 참고하면 됨).
int Neighbors(G *g, int idx_v_source, int *out_idxs_v_target)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *ptr;
	int count = 0;

	for ( ptr = g->e[idx_v_source].next; ptr != 0; ptr = ptr->next )
	{
		*out_idxs_v_target = ptr->idx_v_to;
		++out_idxs_v_target;
		++count;
	}

	return count;
#else
	// Adjacency matrix
	int idx_v_target;
	int count = 0;

	for ( idx_v_target = 0; idx_v_target < g->n_V; ++idx_v_target )
		if ( g->e[idx_v_source][idx_v_target] != 0 )
		{
			*out_idxs_v_target = idx_v_target;
			++out_idxs_v_target;
			++count;
		}

	return count;
#endif
}



// Graph에 있는 v[idx] object에 대한 포인터 값을 return합니다.
// 만약 해당 object가 존재하지 않는 경우 null 포인터 값을 return합니다.
// 사실 얘는 아래에 있는 Get_E()와 대칭성을 갖추기 위해 만들어 놨어요.
// 그냥 g->v[idx_v] 하면 되기 때문에 그리 자주 쓰이지 않을 듯 해요.
V *Get_V(G *g, int idx_v)
{
	if ( idx_v < 0 || idx_v >= g->n_V )
		return 0;

	return &g->v[idx_v];
}

// Graph에 있는 v[from] -> v[to] edge object에 대한 포인터 값을 return합니다.
// 만약 해당 object가 존재하지 않는 경우 null 포인터 값을 return합니다.
// Adjacency list를 쓰는 경우 이 함수를 쓰는 것도 나쁘지 않을 듯 해요.
// 주의: Undirected graph일 때는 모든 edge들이 두 개의 directed edge로 간주되어 들어가 있어요.
//       Edge의 값을 바꾸거나 할 때는 이 점을 유의해야 해요.
E *Get_E(G *g, int idx_v_from, int idx_v_to)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	E_node *ptr;

	for ( ptr = g->e[idx_v_from].next; ptr != 0; ptr = ptr->next )
		if ( ptr->idx_v_to == idx_v_to )
			return &ptr->e;

	return 0;
#else
	// Adjacency matrix
	if ( idx_v_from < 0 || idx_v_from >= g->n_V ||
		idx_v_to < 0 || idx_v_to >= g->n_V )
		return 0;

	return &g->e[idx_v_from][idx_v_to];
#endif
}



// 주어진 파일(키보드 포함)에서 숫자들을 읽어 새 graph를 만들어 return합니다.
// - 현재 숫자 읽는 순서는...
//		> 첫 번째 숫자: Vertex 수
//		> 두 번째 숫자: Edge 수
//		> 이후로 숫자를 두 개씩 끊어 읽어 가며 edge를 만들어 추가합니다.
//		  이 때 isUndirectedGraph 값이 nonzero면 양방향 edge를 각각 하나씩,
//		  값이 zero면 앞 idx -> 뒤 idx 방향 edge 하나를 만듭니다.
//
// TODO: 문제에 따라 입력 방식이 조금씩 다를테니 그 때 그 때 조금씩 수정해 쓰면 될 듯!
G *Load_Graph(FILE *fp, int isUndirectedGraph)
{
	int n_E;
	int input_left, input_right;
	G *g_new;

	// 첫 두 숫자는 각각 n_V, n_E
	fscanf(fp, "%d%d", &input_left, &n_E);

	g_new = New_Graph(input_left);

	while ( n_E > 0 )
	{
		// 다음 숫자들은 모두 edge 정보들
		fscanf(fp, "%d%d", &input_left, &input_right);

		Add_Edge(g_new, input_left, input_right);

		if ( isUndirectedGraph != 0 )
			Add_Edge(g_new, input_right, input_left);

		n_E = n_E - 1;
	}

	return g_new;
}


// 주어진 파일(콘솔 창 포함)에 해당 graph와 관련한 정보를 print해요.
// 초기 버전은 단순 디버그 용도로 쓸 수 있도록 구성해 두었어요.
// 문제에 따라 출력 방식은 매우 많이 다르니 사실상 출력 기능은 main()에서 짜게 될 듯 해요.
void Print_Graph(FILE *fp, G *g)
{
#ifdef USE_ADJACENCY_LIST
	// Adjacency list
	int idx_from;
	E_node *ptr;

	for ( idx_from = 0; idx_from < g->n_V; ++idx_from )
	{
		fprintf(fp, "V[%d](dist : %d): [", idx_from,g->v[idx_from].distance); // 각 vertex의 shortest distance 출력하기 위해 수정

		if ( g->e[idx_from].next != 0 )
		{
			for ( ptr = g->e[idx_from].next; ptr != 0; ptr = ptr->next )
				fprintf(fp, "%d, ", ptr->idx_v_to);

			fprintf(fp, "\b\b]\n");
		}
		else
			fprintf(fp, "empty]\n");
	}
#else
	// Adjacency matrix
	int idx_from;
	int idx_to;
	for ( idx_from = 0; idx_from < g->n_V; ++idx_from )
	{
		fprintf(fp, "V[%d]: [", idx_from);
		for ( idx_to = 0; idx_to < g->n_V; ++idx_to )
			fprintf(fp, "%d, ", g->e[idx_from][idx_to]);
		fprintf(fp, "\b\b]\n");
    }
#endif
}


/*
	Note.

	이제 이 아래에 main()을 비롯한 실제 프로그램 코드를 적으면 될 듯 해요.

	오늘 Graph 관련 함수를 잘 구비해 두면,
	당분간은 코드 위에 있는 vertex / edge 설정만 건드려 가며 문제를 풀 수 있을 거예요!

*/

#define MAX_SIZE 1000

typedef struct Queue { // 배열로 구현한 큐
	int queue[MAX_SIZE + 1];
	int rear;
	int front;
}Queue;

void initQueue(Queue* q) // 큐 초기화
{
	q->front = 0;
	q->rear = 0;
}

int isFull(Queue* q) // 큐 가득차 있는지 확인
{
	if ((q->rear + 1) % MAX_SIZE == q->front)
		return 1;
	else
		return 0;
}

int isEmpty(Queue* q) // 큐가 비었는지 확인
{
	if (q->front == q->rear)
		return 1;
	else
		return 0;
}

void enqueue(Queue* q, int data) // 큐에 넣기
{
	if (isFull(q))
		printf("큐 가득참\n");
	else
	{
		q->queue[q->rear] = data;
		q->rear = (q->rear + 1) % MAX_SIZE;
	}
}
int dequeue(Queue* q) // 큐에서 빼기
{
	int tmp = -1;
	if (isEmpty(q))
		printf("큐 비어있음\n");
	else
	{
		tmp = q->queue[q->front];
		q->front = (q->front + 1) % MAX_SIZE;
	}
	return tmp;
}

void BFS(G *g, int idx_s)
{
	int idx;
	int v_target;
	int* idxs_v_target;
	idxs_v_target = malloc(sizeof(int) * g->n_V);
	Queue q;

	for (idx = 0; idx < g->n_V; ++idx) // 모든 vertex들 초기화 -> Init_Vertex함수와 동일한 역할
	{
		g->v[idx].state = V_WHITE;
		g->v[idx].distance = 0x7FFFFFFF;
		g->v[idx].idx_v_from = -1;  
	}
	g->v[idx_s].state = V_GRAY;
	g->v[idx_s].distance = 0;
	g->v[idx_s].idx_v_from = -1; // ppt에서 파이에 해당함

	initQueue(&q);
	enqueue(&q, idx_s); // 시작점인 vertex를 큐에 넣으면서 시작
	
	while (isEmpty(&q) != 1)
	{
		v_target = dequeue(&q);
		int idx_v_to;

		int result = Neighbors(g, v_target, idxs_v_target);

		for (idx_v_to = 0; idx_v_to < result; ++idx_v_to)
		{
			int idx_adjV = idxs_v_target[idx_v_to]; // 이웃한 vertex들 하나하나 state확인하며 큐에 넣기
			
			if (g->v[idx_adjV].state == V_WHITE)
			{
				g->v[idx_adjV].state = V_GRAY;
				g->v[idx_adjV].distance = g->v[v_target].distance + 1;
				g->v[idx_adjV].idx_v_from = v_target;
				enqueue(&q, idx_adjV);
			}
		}

		g->v[v_target].state = V_BLACK; // 이 vertex의 이제 고정됨
	}
}

void print_path(G* g, int idx_s, int idx_v) // 재귀호출로 s부터 시작하여 path 출력
{
	if (idx_v == idx_s)
		printf("v[%d] ", idx_s);
	else if (g->v[idx_v].idx_v_from == -1)
		printf("no path from %d to %d \n", idx_s, idx_v);
	else
	{
		print_path(g, idx_s, g->v[idx_v].idx_v_from);
		printf("v[%d] ", idx_v);
	}
}

// 과제용 main함수
int main()
{
	FILE *fp = fopen("input_hw2_yeshi.txt", "r"); // input_hw2_yeshi.txt파일이 프로젝트 내에 있어야 함

	G *g = Load_Graph(fp, 0); //ppt예제는 directed 그래프임

	BFS(g, 0); // 과제 함수1

	Print_Graph(stdout, g); // 각 vertex의 shortest distance도 출력하도록 수정하였음

	printf("path : ");
	print_path(g, 0, 11); // 과제 함수2
	printf("\n");

	return 0;
}

/* // 교수님의 main함수
int main()
{
int idx_v_from;
int idx_v_to;
int result;
int *idxs_v_target;

// 주: 아래의 두 문장들 중 하나를 선택하여 키보드 또는 파일에서 graph 생성용 정보를 받아올 수 있어요.
FILE *fp = fopen("input.txt", "r");
//FILE *fp = stdin;

G *g = Load_Graph(fp, 0);

idxs_v_target = malloc(sizeof(int) * g->n_V);

printf("Graph overview:\n");
Print_Graph(stdout, g);


printf("Adjacent() test.\n");

while (1)
{
idx_v_from = -1;
idx_v_to = -1;
printf("Enter idx_v_from and idx_v_to | -1 -1 to quit>");
scanf("%d%d", &idx_v_from, &idx_v_to);

if (idx_v_from == -1 || idx_v_to == -1)
break;

result = Adjacent(g, idx_v_from, idx_v_to);

if (result != 0)
printf("Exists.\n");
else
printf("Not exists.\n");
}

printf("Done.\n");


printf("Neighbors() test.\n");

while (1)
{
idx_v_from = -1;
printf("Enter idx_v_source | -1 to quit>");
scanf("%d", &idx_v_from);

if (idx_v_from == -1)
break;

result = Neighbors(g, idx_v_from, idxs_v_target);

printf("V[%d]\'s neighbor list: [", idx_v_from);

if (result != 0)
{
for (idx_v_to = 0; idx_v_to < result; ++idx_v_to)
printf("%d, ", idxs_v_target[idx_v_to]);

printf("\b\b]\n");
}
else
printf("empty]\n");
}

printf("Done.\n");


Delete_Graph(g);
g = 0;

free(idxs_v_target);

return 0;
}
*/
