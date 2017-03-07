#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int const ATTR_NONE = 0;
int const ATTR_PRIORITY = 1;
int const ATTR_WCET = 2;

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

//show the list of task
void showList(LIST *_head){
	LIST *conductor;
	conductor = _head;
	while(conductor != NULL){
		showTask(conductor -> task);
		conductor = conductor -> next;
	}
}

//add a task based on a formatted buffer string
void addTask(char * buffer){
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tPeriod, &tWCET, &tPriority);

	TASK task = {{0}, tPeriod, tWCET, tPriority, 0};

	strcpy(task.tID, tID);

	add(task);
}

//ISSUE SOLVED: forget to update tail node
//basically, when I insert new node somewhere in the list and not at the end,
//the tail node doesnt get updated and the tail -> next point to somewhere that's
//not NULL (since it's the tail, it should not have anything after it)
//Solution was to check whether the tail -> next is NULL or not, if not tail = its next value
//it yes, then exit the loop
void insertIntoList(LIST *conductor, TASK task){
	LIST *newNode;
	newNode = malloc(sizeof(LIST));

	if(conductor == head){
		newNode -> task = head -> task;
		head -> task = task;
		newNode -> next = head -> next;
		head -> next = newNode;
	}else{
		LIST *previousNode;
		previousNode = head;
		while(previousNode != NULL){
			if(previousNode -> next == conductor){
				newNode -> task = task;
				newNode -> next = conductor;
				previousNode -> next = newNode;
				break;
			}
			previousNode = previousNode -> next;
		}
	}

	while(tail-> next != NULL){
		tail = tail -> next;
	}
	// printf("tail: \n");
	// showList(tail);
	// printf("\n");


}

//prototype for add task and sort by its designated attr
void addTaskSortByAttr(char * buffer, int attr){
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tPeriod, &tWCET, &tPriority);

	TASK task = {{0}, tPeriod, tWCET, tPriority, 0};
	strcpy(task.tID, tID);

	LIST *conductor;

	//use switch case
	switch(attr){
		case 0:
			add(task);
		case 1:
			if(head == NULL){
				add(task);
				conductor = head;
			}else{
				conductor = head;
				while(conductor != NULL){
					if(conductor -> task.tPriority > task.tPriority){
						insertIntoList(conductor, task);
						break;
					}
					conductor = conductor -> next;
				}
			}
			if(conductor == NULL){
				add(task);
			}
		case 2:
			if(head == NULL){
				add(task);
				conductor = head;
			}else{
				conductor = head;
				while(conductor != NULL){
					if(conductor -> task.tWCET > task.tWCET){
						insertIntoList(conductor, task);
						break;
					}
					conductor = conductor -> next;
				}
			}
			if(conductor == NULL){
				add(task);
			}					
	}
}

//add task and add specifically for SJF
void addTaskSJF(char * buffer){
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tPeriod, &tWCET, &tPriority);

	TASK task = {{0}, tPeriod, tWCET, tPriority, 0};
	strcpy(task.tID, tID);

	LIST *conductor;
	
	if(head == NULL){
		add(task);
		conductor = head;
	}else{
		conductor = head;
		while(conductor != NULL){
			if(conductor -> task.tWCET > task.tWCET){
				insertIntoList(conductor, task);
				break;
			}
			conductor = conductor -> next;
		}
	}

	if(conductor == NULL){
		add(task);
	}

	// printf("conductor: \n");
	// showList(conductor);

	// printf("head: \n");
	// showList(head);
	// printf("\n");
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

//file reader exclusively for SJF
void readFileForSJF(char *filename){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTaskSJF(buffer);
	}

}

//prototype for read file and sort by wanted attr
void readFilePrototype(char *filename, int attr){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTaskSortByAttr(buffer, attr);
	}

}

//swap the data of each node, but the pointer
void swapNodeData(LIST *node1, LIST *node2){
	TASK temp = node1 -> task;
	node1 -> task = node2 -> task;
	node2 -> task = temp;
}

//start Shortest Job First Scheduling (using bubble sort)
//currently not needed
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

//as the name suugest, turn list to nil
void resetList(){
	head = tail = NULL;
}

int main(){
	readFilePrototype("input.txt", 1);

	// shortestJobFirst();

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