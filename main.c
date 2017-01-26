#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	char tID[5];
	unsigned int tPeriod;
	unsigned int tWCET;
	unsigned int tPriority;
	unsigned int tWaitTime;
}TASK;

void swapTask(TASK *task1, TASK *task2){
	TASK temp;
	temp = *task1;
	*task1 = *task2;
	*task2 = temp;
}

void calculateWaitTime(TASK *task, int size){
	for(int i = 1; i < size; i++){
		for (int j = i - 1; j >= 0; j--){
			task[i].tWaitTime += task[j].tWCET;
		}
	}
}

void shortestJobFirst(TASK *task, int size){
	for (int i = 0; i < size - 1; i++){
		for (int j = i; j < size; j++){
			if (task[i].tWCET > task[j].tWCET){
					swapTask(&task[i], &task[j]);
			}
		}
	}
}

double averageWaitTime(TASK *task, int size){
	double sum = 0;
	for(int i = 0; i < size; i++){
		for(int j = i-1; j >= 0; j--){
			sum += task[j].tWCET;
		}
	}
	return sum/size;
}

double averageTurnAroundTime(TASK *task, int size){
	double turnAroundTime;
	for(int i = 0; i < size; i++){
		turnAroundTime += task[i].tWCET + task[i].tWaitTime; 
	}

	return turnAroundTime/size;
}

void printTask(TASK *task, int size){
	for(int i = 0; i < size; i++){
		printf("%s %d %d %d %d\n", task[i].tID, task[i].tPeriod, task[i].tWCET, task[i].tPriority, task[i].tWaitTime);
	}	
}

int main(){
	char id[5];
	int value_1= 0, value_2= 0, value_3= 0;
	int i = 0,line = 0;

	char buffer[255];
	FILE *fp;

	fp = fopen("input.txt", "r");

	//read how many tasks the file has

	while(fgets(buffer, sizeof(buffer), fp) != NULL){
		line++;
	}

	printf("Number of lines: %d\n", line);

	TASK * list = malloc(line * sizeof * list);
	
	rewind(fp);
	
	//read each task's data into array of task

	while (fgets(buffer, sizeof(buffer), fp) != NULL){

		sscanf(buffer, "%s %d %d %d", id, &value_1, &value_2, &value_3);
	
		strcpy(list[i].tID, id);
		list[i].tPeriod = value_1;
		list[i].tWCET = value_2;
		list[i].tPriority = value_3;
		list[i].tWaitTime = 0;

		i++;
	}

	printf("Read complete\n");

	//sort the task in order of execution time
	shortestJobFirst(list, line);

	printf("Sort complete\n");

	//calculate each task's wait time
	calculateWaitTime(list, line);

	//average wait time
	printf("%f\n",averageWaitTime(list, line));

	//avaerage turn around time
	printf("%f\n",averageTurnAroundTime(list, line));

	printTask(list, line);

	fclose(fp);

	return 0;
}