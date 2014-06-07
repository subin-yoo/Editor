#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void DataInsertion(List* list, char key)
{
	
	Node* newNode = (Node*)malloc(sizeof(Node)); 
	newNode->data = key;
	newNode->next = NULL; 

	if(list->head ==NULL){
		list->head = newNode;
	} 
	else{
		list->tail->next = newNode;
	}
 	list->tail = newNode; 

	return;
}//DataInsertioin

void Print_key(List* list)
{
	fputc(list->tail->data,stdout);
	return ;
}//Print_key

void delete_key(List* list)
{
	Node* cur = list->head; 
	Node* delNode = NULL;
	Node* prevNode = NULL;
	
	if(list->head == NULL)
	{
		return;
	}
	else
	{
		cur = list->head;
	
		while(cur->next!=NULL)
		{
			prevNode = cur;
			cur = cur->next;
		}
		delNode = cur;
		free(delNode);
		prevNode->next = NULL;
		list->tail = prevNode;
	}
	return ;
}

void Saved_list(List* list)
{
	Node* cur = NULL;
	cur = list->head;
	FILE* fp = fopen("test.txt","wt");
	if(fp == NULL)
	{
		puts("Fail to open file!");
		return;
	}		
	fputc(cur->data,fp);

	while(cur->next!=NULL)
	{
		cur = cur->next; 
		fputc(cur->data,fp);
	}
	fclose(fp);
	return;
}

void Delete_all_node(List* list)
{
	if(list->head ==NULL)
	{	
		return;
	}/*node to be removed does not exit, 
		terminate Delete_all_node*/
	else
	{
		Node* delNode = list->head;
		Node* delNextNode = list->head->next;
		free(delNode); //delete the first node
		list->head == NULL;

		while(delNextNode !=NULL)
		{
			delNode = delNextNode;
			delNextNode = delNextNode->next;
			free(delNode);
		}
		list->tail == NULL;
	}
	return ;
}
