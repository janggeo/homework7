/*
 *  doubly-linked-list.c
 *
 *  Doubly Linked List
 *
 *  Data Structures
 *  Department of Computer Science 
 *  at Chungbuk National University
 *
 */

/* 
	*수정사항*
	1)초기화(initialize함수)를 먼저 사용하지 않고 insertnode를 사용해도
	  오류가 발생하지 않도록 main함수에서 동적할당을 한다
	2)insertNode함수에서 새로운 node를 중간이나 마지막에 삽입하는 경우 
	  n->llink가 새로운node를 가리키지 않는 문제 수정	
	3)deleteFirst함수에서 노드가 남아있는경우,n을 해제한후 연결리스트 제일 앞 노드의 앞 쪽은 비어있으므로 llink를 NULL로 수정
	4)프로그램을 실행하면서 노드의 순서가 어지럽게된 경우 오름차순으로 정렬해주는 함수 추가
*/	


#include<stdio.h>
#include<stdlib.h>
/* 필요한 헤더파일 추가 if necessary */


typedef struct Node {//노드 구조체
	int key;
	struct Node* llink;	//왼쪽 노드를 가리킬 링크
	struct Node* rlink;	//오른쪽 노드를 가리킬 링크
} listNode;

typedef struct Head {	//연결리스트 헤드노드를 가리킬포인터를 요소로 가지는 구조체
	struct Node* first;
}headNode;

/* 함수 리스트 */

/* note: initialize는 이중포인터를 매개변수로 받음
         lab3의 initialize와 차이점을 이해 할것 */
int initialize(headNode** h);
/* 반환값을 int형으로 지정하여 이전 singly-linked-list와는 다르게 
동적할당한headnode를 반환하지 않고도 사용할 수 있게 매개변수로 이중포인터 사용.*/

/* note: freeList는 싱글포인터를 매개변수로 받음
        - initialize와 왜 다른지 이해 할것
        - 이중포인터를 매개변수로 받아도 해제할 수 있을 것 */
int freeList(headNode* h);	

int insertNode(headNode* h, int key);	// 리스트를 검색하여, 입력받은 key보다 큰값이 나오는 노드 바로 앞에 삽입하는 함수
int insertLast(headNode* h, int key);	// 리스트 마지막에 노드를 추가하는 함수
int insertFirst(headNode* h, int key);	// 리스트 제일 앞에 노드를 추가하는 함수
int deleteNode(headNode* h, int key);	// key값에 해당되는 노드 한개를 제거하는 함수
int deleteLast(headNode* h);			//리스트 마지막 노드를 제거하는 함수
int deleteFirst(headNode* h);			//리스트 제일 앞 노드를 제거하는 함수
int invertList(headNode* h);			//리스트를 역순으로 만드는 함수
void printList(headNode* h);			//리스트를 출력하는 함수
headNode* SortList(headNode* h);

int main()
{
	printf("-----[2021076029] [장정환]-----\n");
	char command;
	int key;
	/*리스트 head를 가리킬 노드를 동적할당 후 초기화*/
	headNode* headnode=(headNode*)malloc(sizeof(headnode));	
	headnode->first=NULL;
	do{
		printf("----------------------------------------------------------------\n");
		printf("                     Doubly Linked  List                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize    = z           Print         = p \n");
		printf(" Insert Node   = i           Delete Node   = d \n");
		printf(" Insert Last   = n           Delete Last   = e\n");
		printf(" Insert First  = f           Delete First  = t\n");
		printf(" Invert List   = r           Quit          = q\n");
		printf(" Sort list	= s\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initialize(&headnode);		//headnode의 주소를 넘겨준다. 따라서 매개변수는 이중포인터로 받는다.
			break;
		case 'p': case 'P':
			printList(headnode);		//연결리스트 출력
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insertNode(headnode, key);	//연결리스트에 오름차순으로 노드 삽입
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(headnode, key);	//해당 되는 key값을 가지는 노드 한개 제거
			break;
		case 'n': case 'N':
			printf("Your Key = ");
			scanf("%d", &key);
			insertLast(headnode, key);	//리스트 마지막에 노드 추가
			break;
		case 'e': case 'E':
			deleteLast(headnode);		//리스트 마지막 노드 제거
			break;
		case 'f': case 'F':
			printf("Your Key = ");
			scanf("%d", &key);
			insertFirst(headnode, key);	//리스트 제일 앞에 노드 추가
			break;
		case 't': case 'T':
			deleteFirst(headnode);		//리스트 제일 앞 마지막 제거
			break;
		case 'r': case 'R':
			invertList(headnode);		//리스트 역순으로 배열
			break;
		case 'q': case 'Q':
			freeList(headnode);			//리스트 해제
			break;
		case 's': case'S':
			headnode=Sort(headnode);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}


int initialize(headNode** h) {

	/* headNode가 NULL이 아니면, freeNode를 호출하여 할당된 메모리 모두 해제 */
	if(*h != NULL)
		freeList(*h);

	/* headNode에 대한 메모리를 할당하여 리턴 */
	*h = (headNode*)malloc(sizeof(headNode));
	(*h)->first = NULL;
	return 1;
}

int freeList(headNode* h){
	/* h와 연결된 listNode 메모리 해제
	 * headNode도 해제되어야 함.
	 */
	listNode* p = h->first;

	listNode* prev = NULL;
	while(p != NULL) {
		prev = p;
		p = p->rlink;
		free(prev);
	}
	free(h);
	return 0;
}


void printList(headNode* h) {
	int i = 0;		//노드 개수 count
	listNode* p;	//연결리스트 상에서 옮겨다닐 포인터

	printf("\n---PRINT\n");

	if(h == NULL) {
		printf("Nothing to print....\n");
		return;
	}

	p = h->first;
	/*p를 마지막 노드까지 옮기며 print*/
	while(p != NULL) {
		printf("[ [%d]=%d ] ", i, p->key);
		p = p->rlink;
		i++;
	}

	printf("  items = %d\n", i);
}




/**
 * list에 key에 대한 노드하나를 추가
 */
int insertLast(headNode* h, int key) {
	/*새로운 노드 동적할당*/
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->key = key;
	node->rlink = NULL;
	node->llink = NULL;
	/*이전에 생성된 노드가 없는경우*/
	if (h->first == NULL)
	{
		h->first = node;
		return 0;
	}
	/*연결리스트상에서 옮겨다니며 노드를 가리킬 표인터*/
	listNode* n = h->first;
	while(n->rlink != NULL) {//n을 리스트상 마지막 노드까지 옮긴다.
		n = n->rlink;
	}
	/*node를 연결리스트의 마지막에 추가*/
	n->rlink = node;	//리스트의 마지막을 가리키는 n의 rlink가 새로운node를 가리키도록 한다.
	node->llink = n;	//새로운 node의 llink가 원래 리스트의 마지막을 가리키는 n을 가리키게 한다.
	return 0;
}



/**
 * list의 마지막 노드 삭제
 */
int deleteLast(headNode* h) {
	/*연결리스트가 비어있는 경우*/
	if (h->first == NULL)
	{
		printf("nothing to delete.\n");
		return 0;
	}
	/*리스트상에서 옮겨다니며 노드를 가리킬 포인터 n*/
	listNode* n = h->first;
	/*n전의 노드를 가리킬 포인터*/
	listNode* trail = NULL;

	/* 노드가 하나만 있는 경우, 즉 first node == last node인  경우 처리 */
	if(n->rlink == NULL) {
		h->first = NULL;
		free(n);
		return 0;
	}

	/* 마지막 노드까지 이동 */
	while(n->rlink != NULL) {
		trail = n;
		n = n->rlink;
	}

	/* n이 삭제되므로, 이전 노드의 링크 NULL 처리 */
	trail->rlink = NULL;
	free(n);

	return 0;
}



/**
 * list 처음에 key에 대한 노드하나를 추가
 */
int insertFirst(headNode* h, int key) {
	/*새로운 노드 동적할당*/
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->key = key;
	node->rlink = node->llink = NULL;
	/*이전에 저장된 노드가 없는 경우*/
	if(h->first == NULL)
	{
		h->first = node;
		return 1;
	}
	/*새로운 노드를 리스트앞쪽에 추가 후 링크수정*/ 
	node->rlink = h->first;
	node->llink = NULL;
	listNode *p = h->first;
	p->llink = node;
	/*리스트의 first를 가리키는 링크 수정*/
	h->first = node;

	return 0;
}

/**
 * list의 첫번째 노드 삭제
 */
int deleteFirst(headNode* h) {
	/*이전에 저장된 노드가 없을 경우*/
	if (h->first == NULL)
	{
		printf("nothing to delete.\n");
		return 0;
	}
	/*리스트 앞을 가리키는 포인터를 다음 노드로 옮기고
	리스트 제일 앞을 가리키는 또다른 포인터 n을 free*/
	listNode* n = h->first;
	h->first = n->rlink;

	free(n);
	/*리스트가 남아있는경우, 리스트 제일 앞 노드의 앞 쪽은 비어있으므로 llink를 NULL로*/
	if(h->first!=NULL)
		h->first->llink=NULL;

	return 0;
}



/**
 * 리스트의 링크를 역순으로 재 배치
 */
int invertList(headNode* h) {
	/*이전에 저장된 노드가 없는 경우*/
	if(h->first == NULL) {
		printf("nothing to invert...\n");
		return 0;
	}
	/*리스트상에서 한칸씩 옮겨다니며 노드를 가리킬 포인터들*/
	listNode *n = h->first;
	listNode *trail = NULL;
	listNode *middle = NULL;

	while(n != NULL){	//middle이 가리키는 노드의 좌우 링크를 반전
		/*포인터를 한칸씩 옮긴다.*/
		trail = middle;
		middle = n;
		n = n->rlink;
		/*middle이 가리키는 노드 좌우 반전*/
		middle->rlink = trail;
		middle->llink = n;
	}
	/*middle이 가리키는 마지막 노드를 first로*/
	h->first = middle;

	return 0;
}



/* 리스트를 검색하여, 입력받은 key보다 큰값이 나오는 노드 바로 앞에 삽입 */
int insertNode(headNode* h, int key) {
	/*새로운 노드 동적할당*/
	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->key = key;
	node->llink = node->rlink = NULL;
	/*이전에 저장한 노드가 없는 경우*/
	if (h->first == NULL)
	{
		h->first = node;
		return 0;
	}

	listNode* n = h->first;		//headnode를 가리키는 포인터는 변하면 안되기 때문에 n으로 대신 사용

	/* key를 기준으로 삽입할 위치를 찾는다 */
	while(n != NULL) {
		if(n->key >= key) {
			/* 첫 노드 앞쪽에 삽입해야할 경우 인지 검사 */
			if(n == h->first) {
				insertFirst(h, key);
			} else { /* 중간이거나 마지막인 경우 */
				//새로운 노드를 n이 가리키는 노드와 n이전의 노드 사이에 들어가도록 link를 수정
				node->rlink = n;
				node->llink = n->llink;
				//n왼쪽에 있던 링크가 새로운 노드를 가리키도록 rlink를 수정, n의 왼쪽을 가리키는 llink가 새로운 노드를 가리키도록 수정
				n->llink->rlink = node;
				n->llink=node;
			}
			return 0;
		}

		n = n->rlink;		//n을 연결리스트상에서 한칸씩 옮기며 탐색
	}

	/* 마지막 노드까지 찾지 못한 경우, 마지막에 삽입 */
	insertLast(h, key);
	return 0;
}


/**
 * list에서 key에 대한 노드 삭제
 */
int deleteNode(headNode* h, int key) {
	/*이전에 저장된 노드가 없는 경우*/
	if (h->first == NULL)
	{
		printf("nothing to delete.\n");
		return 1;
	}

	listNode* n = h->first;

	while(n != NULL) {
		if(n->key == key) {
			if(n == h->first) { /* 첫 노드째 노드 인경우 */
				deleteFirst(h);
			} else if (n->rlink == NULL){ /* 마지막 노드인 경우 */
				deleteLast(h);
			} else { /* 중간인 경우 */
				/*n이 가리키는 노드의 좌우 노드를 서로 연결*/
				n->llink->rlink = n->rlink;
				n->rlink->llink = n->llink;
				/*n 해제*/
				free(n);
			}
			return 1;
		}
		/*n을 연결리스트 상에서 다음 노드로 옮긴다*/
		n = n->rlink;
	}

	/* 찾지 못 한경우 */
	printf("cannot find the node for key = %d\n", key);
	return 1;
}

headNode* SortList(headNode* h){
	if(h->first==NULL){
		printf("nothing to sort..\n");
		return 0;
	}
	headNode* hh=(headNode*)malloc(sizeof(headNode));
	hh->first=NULL;

	listNode* n=h->first;
	int min=0;
	while(h->first!=NULL){
		min=n->key;
		while(n->rlink!=NULL){
			n=n->rlink;
			if(n->key<=min){
				min=n->key;
			}
		}
		insertLast(hh,min);
		deleteNode(h,min);
		n=h->first;
	}
	free(h);
	return hh;
}
