#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int const ATTR_NONE = 0;
int const ATTR_ARRIVALTIME = 1;
int const ATTR_WCET = 2;

/*
 *	IF YOU WANT TO PASS A LINK LIST INTO A FUNCTION, ALWAYS PASS ITS REFERENCE
 */

//defining the TASK data structure
typedef struct{
	char tID[5];
	unsigned int tArrivalTime;
	unsigned int tWCET;
	unsigned int tPriority;
	unsigned int tStartTime;
	unsigned int tWaitTime;
}TASK;

//defining the linked list 
//each node contain data + pointer to the next node
typedef struct node{
	TASK task;
	struct node *next;
}LIST;

//show task
void showTask(TASK task){
	printf("%s\t\t%d\t\t\t%d\t\t%d\t\t%d\t\t%d\n", task.tID, task.tArrivalTime, task.tWCET, task.tPriority, task.tStartTime,task.tWaitTime);
}

//show the list of task
void showList(LIST *_head){
	LIST *conductor;
	conductor = _head;
	printf("Task ID\t\tTask Arrival Time\tTask WCET\tTask Priority\tTask Start Time\tTask Wait Time\n");
	while(conductor != NULL){
		showTask(conductor -> task);
		conductor = conductor -> next;
	}
}

//add a TASK to the node
void add(TASK task, LIST **head, LIST **tail){
	LIST *newNode;
	newNode = malloc(sizeof(LIST));
	newNode -> task = task;
	if(*head == NULL){
		// printf("head is null\n");
		*head = *tail = newNode;
		
	}else{
		LIST *newTail = malloc(sizeof(LIST));

		newTail = *tail;
	
		newTail -> next = newNode;
		newTail = newNode;
		newTail -> next = NULL;
		*tail = newTail;
	}
}

//add a task based on a formatted buffer string
void addTask(char * buffer, LIST **head, LIST **tail){
	char tID[5];
	unsigned int tArrivalTime;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tArrivalTime, &tWCET, &tPriority);

	TASK task = {{0}, tArrivalTime, tWCET, tPriority, 0, 0};

	strcpy(task.tID, tID);

	add(task, head, tail);
}

//ISSUE SOLVED: forget to update tail node
//basically, when I insert new node somewhere in the list and not at the end,
//the tail node doesnt get updated and the tail -> next point to somewhere that's
//not NULL (since it's the tail, it should not have anything after it)
//Solution was to check whether the tail -> next is NULL or not, if not tail = its next value
//it yes, then exit the loop
void insertIntoList(LIST *conductor, TASK task, LIST **head, LIST **tail){
	LIST *newNode;
	newNode = malloc(sizeof(LIST));

	if(conductor == *head){
		LIST *newHead = malloc(sizeof(LIST));
		newHead = *head;
		
		newNode -> task = newHead -> task;
		newHead -> task = task;
		newNode -> next = newHead -> next;
		newHead -> next = newNode;

		*head = newHead;
	}else{
		LIST *previousNode;
		previousNode = *head;
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

	LIST *newTail = malloc(sizeof(LIST));
	newTail = *tail;
	while(newTail-> next != NULL){
		newTail = newTail -> next;
		if(newTail -> next == NULL){
			*tail = newTail;
		}
	}
}

//prototype for add task and sort by its designated attr
void addTaskSortByAttr(char * buffer, int attr, LIST **head, LIST **tail){
	char tID[5];
	unsigned int tArrivalTime;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tArrivalTime, &tWCET, &tPriority);

	TASK task = {{0}, tArrivalTime, tWCET, tPriority, 0, 0};
	strcpy(task.tID, tID);

	LIST *conductor;

	// printf("current head is: ");
	// showList(head);

	//use switch case
	switch(attr){
		case 0:
			add(task, head, tail);
			break;
		case 1:
			if(head == NULL){
				add(task, head, tail);
				conductor = *head;
			}else{
				conductor = *head;
				while(conductor != NULL){
					if(conductor -> task.tArrivalTime > task.tArrivalTime){
						insertIntoList(conductor, task, head, tail);
						break;
					}
					conductor = conductor -> next;
				}
			}
			if(conductor == NULL){
				add(task, head, tail);
			}
			break;
		case 2:
			if(head == NULL){
				add(task, head, tail);
				conductor = *head;
			}else{
				conductor = *head;
				while(conductor != NULL){
					if(conductor -> task.tWCET > task.tWCET){
						insertIntoList(conductor, task, head, tail);
						break;
					}
					conductor = conductor -> next;
				}
			}
			if(conductor == NULL){
				add(task, head, tail);
			}
			break;					
	}
}

//add task and add specifically for SJF
void addTaskSJF(char * buffer, LIST **head, LIST **tail){
	char tID[5];
	unsigned int tArrivalTime;
	unsigned int tWCET;
	unsigned int tPriority;

	sscanf(buffer, "%s %d %d %d", tID, &tArrivalTime, &tWCET, &tPriority);

	TASK task = {{0}, tArrivalTime, tWCET, tPriority, 0, 0};
	strcpy(task.tID, tID);

	LIST *conductor;
	
	if(*head == NULL){
		add(task, head, tail);
		conductor = *head;
	}else{
		conductor = *head;
		while(conductor != NULL){
			if(conductor -> task.tWCET > task.tWCET){
				insertIntoList(conductor, task, head, tail);
				break;
			}
			conductor = conductor -> next;
		}
	}

	if(conductor == NULL){
		add(task, head, tail);
	}

	// printf("conductor: \n");
	// showList(conductor);

	// printf("head: \n");
	// showList(head);
	// printf("\n");
}

//read the text file
void readFile(char *filename, LIST **head, LIST **tail){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTask(buffer, head, tail);
	}

}

//file reader exclusively for SJF
void readFileForSJF(char *filename, LIST **head, LIST **tail){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTaskSJF(buffer, head, tail);
	}

}

//prototype for read file and sort by wanted attr
void readFilePrototype(char *filename, int attr, LIST **head, LIST **tail){
	char buffer[225];
	FILE *fp;
	
	fp = fopen(filename, "r");

	while(fgets(buffer, sizeof(buffer),fp) != NULL){
		addTaskSortByAttr(buffer, attr, head, tail);
	}

}

//swap the data of each node, but the pointer
void swapNodeData(LIST *node1, LIST *node2){
	TASK temp = node1 -> task;
	node1 -> task = node2 -> task;
	node2 -> task = temp;
}

//calculate the start time of each task (for anything that's not SJF)
void calculateStartTime(LIST *head){
	LIST *pointer_1;
	LIST *pointer_2;
	pointer_1 = head;
	while(pointer_1 != NULL){
		pointer_2 = pointer_1 -> next;
		while(pointer_2 != NULL){
			pointer_2 -> task.tStartTime += pointer_1 -> task.tWCET;
			if(pointer_2 -> task.tStartTime < pointer_2 -> task.tArrivalTime){
				pointer_2 -> task.tStartTime = pointer_2 -> task.tArrivalTime;
			} 
			pointer_2 = pointer_2 -> next;
		}
		pointer_1 = pointer_1 -> next;
	}
}

//calculate the wait time of each task after scheduling
void calculateWaitTime(LIST *head){
	LIST *pointer_1;
	pointer_1 = head;
	while(pointer_1 != NULL){
		pointer_1 -> task.tWaitTime = pointer_1 -> task.tStartTime - pointer_1 -> task.tArrivalTime;
		pointer_1 = pointer_1 -> next;
	}
}

//average wait time
float averageWaitTime(LIST *head){
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

//reset all arrival time to 0
void shortestJobFirst(){
	printf("SHORTEST JOB FIRST ALGORITHM\n");
	
	LIST *head = NULL;
	LIST *tail = NULL;

	readFilePrototype("input.txt", ATTR_WCET, &head, &tail);
	
	LIST *pointer;
	
	pointer = head;
	
	while(pointer != NULL){
		pointer -> task.tArrivalTime = 0;
		pointer = pointer -> next;
	}

	calculateStartTime(head);

	calculateWaitTime(head);
	
	float avgWaitTime = averageWaitTime(head);

	printf("The order of execution of the given task list is: \n");
	printf("*NOTE: WCET = Worst Case Execution Time\n");

	showList(head);
	
	printf("Average Wait Time: %.2f\n", avgWaitTime);

	LIST *cloneOfHead;

	cloneOfHead = head;

	sortListByTaskName(cloneOfHead);

}

void firstComeFirstServe(){
	printf("FIRST COME FIRST SERVE ALGORITHM\n");

	LIST *head = NULL;
	LIST *tail = NULL;

	readFilePrototype("input.txt", ATTR_ARRIVALTIME, &head, &tail);

	calculateStartTime(head);

	calculateWaitTime(head);
	
	float avgWaitTime = averageWaitTime(head);

	printf("The order of execution of the given task list is: \n");
	printf("*NOTE: WCET = Worst Case Execution Time\n");

	showList(head);
	
	printf("Average Wait Time: %.2f\n", avgWaitTime);

	LIST *cloneOfHead;

	cloneOfHead = head;

	sortListByTaskName(cloneOfHead);
}

//BUG TO BE FIX: FRIGGING SET ARRIVAL TIME AND START TIME OF NEW TASK
void shortestRemainingTime(){
	printf("SHORTEST REMAINING TIME ALGORITHM\n");

	LIST *head = NULL;
	LIST *tail = NULL;

	readFilePrototype("input.txt", ATTR_ARRIVALTIME, &head, &tail);

	int currenTaskRemainingTime = 0;

	LIST *pointer_1;
	LIST *pointer_2;

	pointer_1 = head;

	while(pointer_1 != NULL){
		// printf(".");
		showList(pointer_1);
		currenTaskRemainingTime = pointer_1 -> task.tWCET - pointer_1 -> next -> task.tArrivalTime;

		if(currenTaskRemainingTime <= 0){
			pointer_1 -> next -> task.tStartTime = pointer_1 -> task.tStartTime + pointer_1 -> task.tWCET;
			if (pointer_1 -> next -> task.tStartTime < pointer_1 -> next -> task.tArrivalTime){
				pointer_1 -> next -> task.tStartTime = pointer_1 -> next -> task.tArrivalTime;
			}
		}else{
			pointer_1 -> task.tWCET = pointer_1 -> next -> task.tArrivalTime;
			
			TASK newTask = {{0}, 0, currenTaskRemainingTime, pointer_1 -> task.tPriority, 0, 0};
			strcpy(newTask.tID, pointer_1 -> task.tID);

			pointer_2 = pointer_1 -> next -> next;
			while(pointer_2 != NULL){
				// printf("a");
				if(pointer_2 -> task.tWCET > newTask.tWCET){
					insertIntoList(pointer_2, newTask, &head, &tail);
					break;
				}
				pointer_2 = pointer_2 -> next;
			}
		}
		pointer_1 = pointer_1 -> next;
		showList(head);
		sleep(3);
	}
	printf("\n");

	printf("The order of execution of the given task list is: \n");
	printf("*NOTE: WCET = Worst Case Execution Time\n");

	showList(head);
	
	// printf("Average Wait Time: %.2f\n", avgWaitTime);

	LIST *cloneOfHead;

	cloneOfHead = head;

	sortListByTaskName(cloneOfHead);

}

int main(){
	firstComeFirstServe();
	printf("-------------------------------------------------------------------------------------------------------\n");
	shortestJobFirst();
	printf("-------------------------------------------------------------------------------------------------------\n");
	shortestRemainingTime();
	return 0;
}