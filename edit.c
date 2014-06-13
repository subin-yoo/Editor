#include <stdio.h>
#include <stdlib.h>
#include "node.h"

void DataInsertion(List* list, char key)
{
	
	Node* newNode = (Node*)malloc(sizeof(Node)); //Node creation 
	newNode->data = key; //Store the key into node
	newNode->next = NULL; //set next for node to NULL

	if(list->head ==NULL){//If newNode is the first node
		list->head = newNode; //Head points to the first node
	} 
	else{//If newNode is not the first node
		list->tail->next = newNode; //Connection of nodes
	}
 	list->tail = newNode; //tail points to end of node
	
	return;
}//DataInsertioin

void Print_key(List* list)
{
	fputc(list->tail->data,stdout);//display character that tail points 
	return ;
}//Print_key

void delete_key(List* list)
{
	Node* cur;  
	Node* delNode = NULL;
	Node* prevNode = NULL;
	
	if(list->head == NULL)
	{
		return;
	}
	else //If at least one node exits, free the last node and renew the tail  
	{
		cur = list->head;//cur points to the first node
	
		while(cur->next!=NULL)
		{
			prevNode = cur;
			cur = cur->next;
		}//Find the last node
		delNode = cur;
		free(delNode); //free the last node
		prevNode->next = NULL; //Set next of the new last node to NULL 
		list->tail = prevNode; //renew the tail
	}
	return ;
}

void Saved_list(List* list)
{
	Node* cur = NULL;
	cur = list->head; //cur points to head
	FILE* fp = fopen("test.txt","wt"); //Create the stream with 'test.txt' file using write mode 
	if(fp == NULL)
	{
		puts("Fail to open file!");
		return;
	}		
	fputc(cur->data,fp); //Write data in the first node in the file

	while(cur->next!=NULL)
	{
		cur = cur->next; 
		fputc(cur->data,fp);
	}//Write all data in node in the file
	fclose(fp); //free the stream with 'test.txt' file
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
		Node* delNode = list->head;//delNode points to the first node
		Node* delNextNode = list->head->next; //delNextNode points to the node to be deleted in the next step
		free(delNode); //delete the first node
		list->head = NULL; //Set head to NULL

		while(delNextNode !=NULL)
		{
			delNode = delNextNode; //delNode points to node to be deleted
			delNextNode = delNextNode->next; //delNextNode points to node to be the first node
			free(delNode);//Delete the current first node
		}
		list->tail = NULL; //Set tail to NULL after all nodes are deleted
	}
	return ;
}
