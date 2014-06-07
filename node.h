#ifndef __NODE_H__
#define __NODE_H__

//Declaration for node structure
typedef struct Nodestruct{
	char data;
	struct Nodestruct* next;
}Node;

typedef struct{
	Node* head;
	Node* tail;
}List;

#endif
