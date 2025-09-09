#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[10];
    int arrivalTime;
    int burstTime;
    int remainingTime; // For RR and PSJF
    int waitingTime;
    int startTime;
    int endTime;
} Task;

void resetRemainingTime(Task *tasks, int taskCount);
void scheduleFCFS(Task *tasks, int taskCount, const char *outputFile);
void scheduleRR(Task *tasks, int taskCount, const char *outputFile);
void scheduleNSJF(Task *tasks, int taskCount, const char *outputFile);
void schedulePSJF(Task *tasks, int taskCount, const char *outputFile);

int main() {
    Task tasks[50];

    FILE *file = fopen("TaskSpec.txt", "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    int count = 0;
    while (fscanf(file, "%[^,],%d,%d\n", tasks[count].name, &tasks[count].arrivalTime, &tasks[count].burstTime) == 3) {
        tasks[count].remainingTime = tasks[count].burstTime;
        count++;
    }

    if(count<2){
        perror("Not enough processes");
        exit(1);
    }

    fclose(file);

    // Clear output file
    FILE *wfile = fopen("Output.txt", "w");
    fclose(wfile);


    scheduleFCFS(tasks, count, "Output.txt");
    resetRemainingTime(tasks,count);
    scheduleRR(tasks, count, "Output.txt");
    resetRemainingTime(tasks,count);
    scheduleNSJF(tasks, count, "Output.txt");
    schedulePSJF(tasks, count, "Output.txt");

    printf("Results written to Output.txt!\n");
    return 0;
}

void resetRemainingTime(Task *tasks, int taskCount) {
    for (int i = 0; i < taskCount; i++) {
        tasks[i].remainingTime = tasks[i].burstTime;
    }
}


void scheduleFCFS(Task *tasks, int taskCount, const char *outputFile) {
    FILE *file = fopen(outputFile, "a");
    fprintf(file, "FCFS:\n");

    int currentTime= 0;
    double totalTime = 0;
    int startTime;
    int endTime;

    for (int i = 0; i < taskCount; i++) {

        if(currentTime < tasks[i].arrivalTime)
            currentTime = tasks[i].arrivalTime;

        startTime = currentTime;
        endTime = currentTime + tasks[i].burstTime;
        tasks[i].waitingTime = startTime - tasks[i].arrivalTime;

        fprintf(file, "%s\t%d\t%d\n", tasks[i].name, startTime, endTime);
        totalTime += tasks[i].waitingTime;

        currentTime = endTime;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "Waiting Time %s: %d\n", tasks[i].name, tasks[i].waitingTime);
    }
    fprintf(file, "Average Waiting Time: %.2f\n\n",totalTime / taskCount);
    fclose(file);
}
void scheduleRR(Task *tasks, int taskCount, const char *outputFile) {
    FILE *file = fopen(outputFile, "a");
    fprintf(file, "RR:\n");

    const int timeQuantum = 4;
    int currentTime = 0;
    double totalTime = 0;
    int remainingTasks = taskCount;

    while (remainingTasks > 0) {
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].remainingTime > 0 && tasks[i].arrivalTime <= currentTime) {

                //first time picking the task
                if (tasks[i].remainingTime == tasks[i].burstTime) {
                    tasks[i].startTime = currentTime;
                }

                int executionTime = (tasks[i].remainingTime <= timeQuantum) ? tasks[i].remainingTime : timeQuantum;
                currentTime += executionTime;
                tasks[i].remainingTime -= executionTime;

                fprintf(file, "%s\t%d\t%d\n", tasks[i].name, currentTime - executionTime, currentTime);

                if (tasks[i].remainingTime == 0) {
                    tasks[i].endTime = currentTime;
                    tasks[i].waitingTime = tasks[i].endTime - tasks[i].arrivalTime - tasks[i].burstTime;
                    totalTime += tasks[i].waitingTime;
                    remainingTasks--;
                }
            }
        }

        // If all remaining tasks have arrival times greater than current time, move the clock
        int hasFutureTasks = 0;
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].remainingTime > 0 && tasks[i].arrivalTime > currentTime) {
                hasFutureTasks = 1;
                break;
            }
        }
        if (remainingTasks > 0 && hasFutureTasks) {
            currentTime++;
        }
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "Waiting Time %s: %d\n", tasks[i].name, tasks[i].waitingTime);
    }


    double averageWaitingTime = totalTime / taskCount;
    fprintf(file, "Average Waiting Time: %.2f\n\n", averageWaitingTime);

    fclose(file);
}

void scheduleNSJF(Task *tasks, int taskCount, const char *outputFile) {
    FILE *file = fopen(outputFile, "a");
    fprintf(file, "NSJF:\n");

    int currentTime = 0;
    double totalTime = 0;

    int isCompleted[50] = {0}; // Array to track completed tasks
    int completed = 0;

    while (completed < taskCount) {
        int min_index = -1;

        // selects which task has the lowest burst time
        for (int j = 0; j < taskCount; j++) {
            if (!isCompleted[j] && tasks[j].arrivalTime <= currentTime) {
                if (min_index == -1 || tasks[j].burstTime < tasks[min_index].burstTime) {
                    min_index = j;
                }
            }
        }

        int startTime = currentTime;
        int endTime = currentTime + tasks[min_index].burstTime;
        tasks[min_index].waitingTime = startTime - tasks[min_index].arrivalTime;

        currentTime = endTime;
        totalTime += tasks[min_index].waitingTime;

        fprintf(file, "%s\t%d\t%d\n", tasks[min_index].name, startTime, endTime);

        isCompleted[min_index] = 1;
        completed++;
    }


    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "Waiting Time %s: %d\n", tasks[i].name, tasks[i].waitingTime);
    }
    fprintf(file, "\n");


    fprintf(file, "Average Waiting Time: %.2lf\n\n", totalTime / taskCount);
    fclose(file);
}


void schedulePSJF(Task *tasks, int taskCount, const char *outputFile) {
    FILE *file = fopen(outputFile, "a");
    fprintf(file, "PSJF:\n");

    int currentTime = 0;
    double totalTime = 0;
    int endTime;
    int isCompleted[50] = {0};
    int completedCounter = 0;
    int saveIndex = 0;
    int printed = 0;


    while (completedCounter < taskCount) {
        int min_index = -1;

        // selects which task has the lowest remaining time left for task completion, if remaining time is same it checks
        //it selects for the one which has the recent arrival time or whose remaining time is equal to the burst time
        // arrival time, if arrival time is same it checks for order of list
        for (int j = 0; j < taskCount; j++) {
            if (!isCompleted[j] && tasks[j].arrivalTime <= currentTime) {
                if (min_index == -1 ||
                    tasks[j].remainingTime < tasks[min_index].remainingTime ||
                    (tasks[j].remainingTime == tasks[min_index].remainingTime && tasks[j].remainingTime == tasks[j].burstTime) ||
                    (tasks[j].remainingTime == tasks[min_index].remainingTime && tasks[j].arrivalTime < tasks[min_index].arrivalTime) ||
                    (tasks[j].remainingTime == tasks[min_index].remainingTime &&
                     tasks[j].arrivalTime == tasks[min_index].arrivalTime && j < min_index)) {
                    min_index = j;
                }
            }
        }

        if (tasks[min_index].remainingTime == tasks[min_index].burstTime) {
            tasks[min_index].startTime = currentTime;
        } else if (saveIndex != min_index) {
            tasks[min_index].startTime = currentTime; // Update startTime for resumed task
        }


        if (!printed && saveIndex != min_index) {
            fprintf(file, "%s\t%d\t%d\n", tasks[saveIndex].name, tasks[saveIndex].startTime, currentTime);
            printed = 0;
        }

        tasks[min_index].remainingTime--;
        currentTime++;

        if (tasks[min_index].remainingTime == 0) {

            endTime = currentTime;
            fprintf(file, "%s\t%d\t%d\n", tasks[min_index].name, tasks[min_index].startTime, endTime);
            tasks[min_index].waitingTime = endTime - tasks[min_index].arrivalTime - tasks[min_index].burstTime;
            isCompleted[min_index] = 1;
            completedCounter++;
            totalTime += tasks[min_index].waitingTime;
            printed = 1;
        }

        saveIndex = min_index;
    }

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "Waiting Time %s: %d\n", tasks[i].name, tasks[i].waitingTime);
    }

    fprintf(file, "Average Waiting Time: %.2f\n\n", totalTime / taskCount);
    fclose(file);
}
