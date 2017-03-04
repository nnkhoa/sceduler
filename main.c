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
	TASK t;
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
	newNode -> t = task;
	if(head == NULL){
		head = tail = newNode;
	}else{
		tail->next = newNode;
		tail = newNode;
		tail->next = NULL;
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

//has problem here
void showList(){
	LIST *conductor;
	conductor = head;
	while(conductor != NULL){
		showTask(conductor->t);
		conductor = conductor->next;
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

int main(){
	readFile("input.txt");
	showList();
	return 0;
}