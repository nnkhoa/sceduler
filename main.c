#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;
	unsigned int tWaitTime;
}TASK;

typedef struct node{
	TASK task;
	struct node *next;
}LIST;

LIST *head;
LIST *tail;

void initList(){
	head = tail = NULL;
}

void add(TASK task){
	LIST *newNode;
	newNode = malloc(sizeof(LIST));
	newNode -> task = task;
	if(head == NULL){
		head = tail = newNode;
	}else{
		tail -> next = newNode;
		tail = newNode;
		tail -> next = NULL;
	}
	
}

void showTask(TASK task){
	printf("%s\t%d\t%d\t%d\t%d\n", task.tID, task.tPeriod, task.tWCET, task.tPriority, task.tWaitTime);
}

void addTask(char * buffer){
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;
	unsigned int tWaitTime;

	sscanf(buffer, "%s %d %d %d", tID, &tPeriod, &tWCET, &tPriority);

	TASK task = {{NULL}, tPeriod, tWCET, tPriority, 0};

	strcpy(task.tID, tID);

	add(task);
}

void showList(){
	LIST *conductor;
	conductor = head;
	while(conductor != NULL){
		showTask(conductor -> task);
		conductor = conductor -> next;
	}
}

void readFile(char *filename){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTask(buffer);
	}

}

void swapNodeData(LIST *node1, LIST *node2){
	TASK temp = node1 -> task;
	node1 -> task = node2 -> task;
	node2 -> task = temp;
}

void shortestJobFirst(){
	LIST *pointer_1;
	LIST *pointer_2;
	pointer_1 = head;
	while(pointer_1 != NULL){
		pointer_2 = pointer_1 -> next;
		while(pointer_2 != NULL){
			if(pointer_2 -> task.tWCET < pointer_1 -> task.tWCET){
				swapNodeData(pointer_1, pointer_2);
			}
			pointer_2 = pointer_2 -> next;
		}
		pointer_1 = pointer_1 -> next;
	}
}

void calculateWaitTime(){
	LIST *pointer_1;
	LIST *pointer_2;
	pointer_1 = head;
	while(pointer_1 != NULL){
		pointer_2 = pointer_1 -> next;
		while(pointer_2 != NULL){
			pointer_2 -> task.tWaitTime += pointer_1 -> task.tWCET; 
			pointer_2 = pointer_2 -> next;
		}
		pointer_1 = pointer_1 -> next;
	}
}

int main(){
	readFile("input.txt");
	shortestJobFirst();
	calculateWaitTime();
	showList();
	return 0;
}