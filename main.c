#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining the TASK data structure
typedef struct{
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;
	unsigned int tWaitTime;
}TASK;

//defining the linked list 
//each node contain data + pointer to the next node
typedef struct node{
	TASK task;
	struct node *next;
}LIST;

//first and last node of the list
LIST *head;
LIST *tail;

//initiate the list
void initList(){
	head = tail = NULL;
}

//add a TASK to the node
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

//show task
void showTask(TASK task){
	printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n", task.tID, task.tPeriod, task.tWCET, task.tPriority, task.tWaitTime);
}

//add a task based on a formatted buffer string
void addTask(char * buffer){
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tPeriod, &tWCET, &tPriority);

	TASK task = {{NULL}, tPeriod, tWCET, tPriority, 0};

	strcpy(task.tID, tID);

	add(task);
}

//show the list of task
void showList(LIST *_head){
	LIST *conductor;
	conductor = _head;
	while(conductor != NULL){
		showTask(conductor -> task);
		conductor = conductor -> next;
	}
}

//read the text file
void readFile(char *filename){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTask(buffer);
	}

}

//swap the data of each node, but the pointer
void swapNodeData(LIST *node1, LIST *node2){
	TASK temp = node1 -> task;
	node1 -> task = node2 -> task;
	node2 -> task = temp;
}

//start Shortest Job First Scheduling (using bubble sort)
//MAY change scheduling algorithm in the future 
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

//Shortest Job First scheduling algorithm with task period taken into consideration
void sJFWithPeriod(){
	LIST *pointer_1;
	LIST *pointer_2;
	pointer_1 = head;

	pointer_2 = pointer_1 -> next;

	//put the 1st arrived task to the top of the list
	while(pointer_2 != NULL){
		if(pointer_2 -> task.tPeriod < pointer_1 -> task.tPeriod){
			swapNodeData(pointer_2, pointer_1);
		}
	}

	pointer_1 = pointer_1 -> next;
	pointer_2 = pointer_1 -> next;
}

//calculate the wait time of each task after scheduling
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

//average wait time
float averageWaitTime(){
	LIST *conductor;
	float count = 0;
	float sum = 0;
	conductor = head;

	while(conductor != NULL){
		sum += conductor -> task.tWaitTime;
		count++;
		conductor = conductor -> next;
	}

	float avgWaitTime = sum/count;

	return avgWaitTime;
}

//sort and print the task list by name
void sortListByTaskName(LIST *_head){
	LIST *pointer_1;
	LIST *pointer_2;
	pointer_1 = _head;
	while(pointer_1 != NULL){
		pointer_2 = pointer_1 -> next;
		while(pointer_2 != NULL){
			if(strcmp(pointer_2 -> task.tID, pointer_1 -> task.tID) < 0){
				swapNodeData(pointer_2, pointer_1);
			}
			pointer_2 = pointer_2 -> next;
		}
		pointer_1 = pointer_1 -> next;
	}
	showList(_head);
}

int main(){
	readFile("input.txt");

	shortestJobFirst();

	calculateWaitTime();
	
	float avgWaitTime = averageWaitTime();

	printf("The order of execution of the given task list is: \n");
	printf("*NOTE: WCET = Worst Case Execution Time\n");
	printf("Task ID\t\tTask Period\tTask WCET\tTask Priority\tTask Wait Time\n");

	showList(head);
	
	printf("Average Wait Time: %.2f\n", avgWaitTime);

	LIST *cloneOfHead;

	cloneOfHead = head;

	sortListByTaskName(cloneOfHead);

	return 0;
}