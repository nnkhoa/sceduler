Pseudocode Shortest Job First:

open the file
while get string of line not NULL
	line + 1
	(get the number of tasks in the file)

while get string of line not NULL
	read each line into array of task

for (i = 0; i < line - 1; i++)
	for(j = i; j < line; j++)
		if task[i].WorstExecutionTime > task[j].WorstExecutionTime
			swap task[i] and task[j]
endfor
(sort the task in order of lowest execution time to highest)

for (i = 0; i < line - 1; i++)
	for(j = i; j >= 0 ; j--)
		total wait time += task[j].WorstExecutionTime
endfor

average wait time = total wait time / line
(calculate the average wait time of the list of tasks)

total turnaround time = total wait time + total execution time
average turnaround time = total turn around time / line
(calculate the average turnaround time of the list of tasks)