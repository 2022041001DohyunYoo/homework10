/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * School of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;


/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;



int initializeBST(Node** h); //BST 초기화
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* functions that you have to implement */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int deleteNode(Node* head, int key);  /* delete the node for the key */
Node* pop(); //팝 (스택)
void push(Node* aNode); // 푸쉬(스택)
Node* deQueue(); //큐에서 빼기
void enQueue(Node* aNode); //큐에 집어넣기

/* you may add your own defined functions if necessary */


void printStack();//스택 출력



int main()
{
    //명령과 입력값과 헤드노드
	char command;
	int key;
	Node* head = NULL;

		printf("[-----[유도현][2022041001]-----]");
	do{
        //양식
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

        //명령 받기
		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head); //BST 초기화
			break;
		case 'q': case 'Q':
			freeBST(head); //BST 모든 노드 할당해제
			break;
		case 'i': case 'I':
            //입력값 받아서 적절한 위치에 키 삽입
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
            //입력값 받아서 같은 값을 가진 노드 할당해제
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
            //재귀적 방식 중위수노히
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
            //반복적 방식 중위순회
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
            //레벨순서 순회
			levelOrder(head->left);
			break;

		case 'p': case 'P':
            //스택 출력
			printStack();
			break;

		default:
            //정의되지 않은 동작
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	//트리가 비어 있지 않으면 전부 할당 해제
	if(*h != NULL)
		freeBST(*h);

	// 헤드노드 할당
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

    //스택의 탑
	top = -1;


    //큐의 앞과 뒤
	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
    //ptr이 NULL이 아닐 동안 반복
	if(ptr) {
        //재귀호출을 이용한 중위 순회 알고리즘
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)
{
    while(1){//무한 반복

        //node가 NULL이 아닐동안 실행하고 node의 왼쪽 자식노드로 이동
		for(; node; node = node->left)
            //노드 푸쉬
			push(node);
        //노드 팝
		node = pop();

        //노드가 NULL이면 반복 종료
		if(!node) break;
        //현재 노드의 키 값 출력
		printf(" [%d] ", node->key);

        //node에 node의 오른쪽 자식
		node = node->right;
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{

	if(!ptr) return; //트리 비어있으면 종료

	enQueue(ptr);//큐에 ptr 집어넣기

	while(1)//무한 반복
	{
		ptr = deQueue(); //큐에서 노드 하나 빼서 ptr에 저장`
        //ptr이 NULL이 아니면
		if(ptr) {
            //키 값 출력
			printf(" [%d] ", ptr->key);

            //만약 ptr의 왼쪽 자식 노드가 비어있지 않으면
			if(ptr->left)
                //왼쪽 자식 노드를 큐에 넣기
				enQueue(ptr->left);
            //만약 ptr의 오른 자식 노드가 비어있지 않으면
			if(ptr->right)
                //오른 자식 노드를 큐에 넣기
				enQueue(ptr->right);
		}
        //ptr이 NULL이면 반복 종료
		else
			break;

	}
}


int insert(Node* head, int key)
{
    //새 노드 할당
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

    //만약 트리가 비어 있으면 새 노드를 루트 노드로
	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	//ptr에 루트노드를
	Node* ptr = head->left;
    //ptr의 부모 노드를 저장할 변수
	Node* parentNode = NULL;

    //ptr이 NULL이 아닐때 까지 반복
	while(ptr) {

		//만약 중복되는 키 값이 있다면 중지
		if(ptr->key == key) return 1;

		//ptr을 다음 노드로 넘기기 전에 부모노드를 백업
		parentNode = ptr;

		//현재 노드 키값과 입력값을 비교하여 죄우 어느쪽으로 이동할지 정하여 이동
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	//반복 종료 후 = ptr이 NULL이 될때 부모노드의 키 값과 입력값을 비교하여 좌우 적절한 위치에 새 노드 연결
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
    //만약 헤드노드가 비어 있으면
    if (head == NULL) {
		printf("\n Need to initialize \n");
		return -1;
	}
    //만약 루트노드가 비어 있으면
	if (head->left == NULL) {
		printf("\n No node existing\n");
		return -1;
	}

    //root에 루트노드
	Node* root = head->left;



	Node* parent = NULL;
	Node* ptr = root;

    //ptr이 NULL이 아니고 ptr의 키값이 입력값이랑 다를 동안 반복
	while((ptr != NULL)&&(ptr->key != key)) {
        //만약 ptr의 키 값과 입력값이 다르면
		if(ptr->key != key) {

			parent = ptr;	//parent에 ptr의 값 백업

            //만약 ptr의 키값이 입력값보다 크면
			if(ptr->key > key)
                //ptr에 ptr의 왼쪽 자식 노드
				ptr = ptr->left;
			else
                //ptr에 오른쪽 자식 노드
				ptr = ptr->right;
		}
	}

	//ptr이 NULL이면
	if(ptr == NULL)
	{
		printf("입력값 [%d]에 해당하는 노드 없음 \n ", key);
		return -1;
	}

	/*
	 * case 1: 리프노드를 제거 하는 경우
	 */
	if(ptr->left == NULL && ptr->right == NULL)
	{
		if(parent != NULL) { //ptr이 parent의 어느 방향인지 파악하고 parent와 연결 끊기
			if(parent->left == ptr)
				parent->left = NULL;
			else
				parent->right = NULL;
		} else {
			//만약 ptr이 NULL이라면 헤드노드와 ptr 연결 해제
			head->left = NULL;

		}

		free(ptr);//ptr 할당 해제
		return 1;
	}

	/**
	 * case 2: 삭제 될 노드에 자식노드가 하나 있다면
	 */
    //만약 ptr의 좌우노드중 하나, 혹은 그 이하가 NULL이 아니라면
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child;//자식 노드

        //자식노드가 ptr의 왼쪽에 있는지 오른쪽에 있는지 구별해서 child에 저장
		if (ptr->left != NULL)
			child = ptr->left;
		else
			child = ptr->right;

        //만약 ptr이 루트노드가 아니라면
		if(parent != NULL)
		{
            //parant의 어느쪽에 ptr이 있었는지 판별해서 대신 child와 연결
			if(parent->left == ptr)
				parent->left = child;
			else
				parent->right = child;
		} else {
			//만약 ptr이 루트 노드라면
			root = child;//루트노드로 child
		}

		free(ptr);
		return 1;
	}

	/**
	 * case 3: 삭제할 노드가 2개의 자식노드를 가지고 있을 경우
	 *
	 * 오른쪽 서브트리에서 가장 값이 작은 노드를 찾음
	 *
	 */

	Node* candidate;
	parent = ptr;

    //후보노드에 ptr의 오른쪽 자식 노드
	candidate = ptr->right;

	// 오른쪽 서브트리의 가장 작은 노드는 오른 서브 트리의 가장 왼쪽에 존재하는 노드

    //candidate는 서브트리의 가장 왼쪽 노드, parant는 그 부모노드를 가리키게
	while(candidate->left != NULL)
	{
        
		parent = candidate;
		candidate = candidate->left;
	}

	// candinate은 삭제되어야 할 후보 노드

    //후보 노드가 부모노드의 왼쪽에 있는지 오른쪽에 있는지 판별 후 후보노드의 오른 자식 노드를 대신 연결
	if (parent->right == candidate)
		parent->right = candidate->right;
	else
		parent->left = candidate->right;

	//ptr을 삭제하는 대신 후보 노드의 키값으로 교체 후 후보노드를 할당 해제

	ptr->key = candidate->key; //후보노드의 키값을 ptr의 키값으로

	free(candidate);//후보 노드 할당해제
	return 1;
}


void freeNode(Node* ptr)
{
    //ptr이 노드가 아닐동안
	if(ptr) {
        //ptr의 왼쪽과 오른쪽 노드에 재귀호출 후 ptr도 할당 해제
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

    //만약 헤드노드 밖에 없으면
	if(head->left == head)
	{   
        //헤드노드 할당 해제후 종료
		free(head);
		return 1;
	}

    //p에 헤드노드의 왼쪽 노드 = 루트 노드
	Node* p = head->left;

    //p부터 노드 연쇄적으로 할당 해제
	freeNode(p);

    //헤드노드 할당해제
	free(head);
	return 1;
}



Node* pop()
{
    //만약 탑이 0보다 작으면 종료
    if (top < 0) return NULL;
    //탑을 반환하고 탑의 위치를 한칸 아래로
	return stack[top--];
}

void push(Node* aNode)
{
    //탑의 위치를 한칸 위로 올린 후 aNode를 집어넣기
	stack[++top] = aNode;
}

void printStack()
{
	int i = 0;
	printf("--- stack ---\n");

    //탑까지 스택의 모든 요소 출력
	while(i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}


Node* deQueue()
{   
    //만약 큐가 비어 있으면 오류 출력 후 종료
    if (front == rear) {
		return NULL;
	}

    //front를 한칸 앞으로 이동 후 반환
	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];
}

void enQueue(Node* aNode)
{
    //rear를 한칸 앞으로 이동 
    rear = (rear + 1) % MAX_QUEUE_SIZE;

    //만약 큐가 가득 차 있으면 오류 출력 후 종료
	if (front == rear) {
		return;
	}
	
	//aNode를 삽입
	queue[rear] = aNode;
}




