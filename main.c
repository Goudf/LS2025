#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESSES 100

// Define a Processing structure
typedef struct {
    char id[3];
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
    bool completed;
} Processing;

// BubbleSort
void sort_by_arrival(Processing processing[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processing[i].arrival_time > processing[j].arrival_time) {
                Processing temp = processing[i];
                processing[i] = processing[j];
                processing[j] = temp;
            }
        }
    }
}

// Function to calculate times (completion time, turnaround time, waiting time)
void calculate_times(Processing *p, int current_time) {
    p->completion_time = current_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->burst_time;
}

//Display on the screen
void display(Processing processing[], int n) {
    printf("\nProcess\tArrival\tBurst\tPriority\tWaiting\tTurnaround\tCompletion\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processing[i].id,
               processing[i].arrival_time,
               processing[i].burst_time,
               processing[i].priority,
               processing[i].waiting_time,
               processing[i].turnaround_time,
               processing[i].completion_time);
    }
}

// Reset remaining time
void reset_remaining_time(Processing processing[], int n) {
    for (int i = 0; i < n; i++) {
        processing[i].remaining_time = processing[i].burst_time;
        processing[i].completed = false;
    }
}

//First Come First Served (FCFS)
void fcfs(Processing processing[], int n) {
    int current_time = 0;
    printf("\nGantt Chart [First Come First Served(FCFS)]:\n");

    for (int i = 0; i < n; i++) {
        if (current_time < processing[i].arrival_time) {
            printf("| Idle %d ", processing[i].arrival_time);
            current_time = processing[i].arrival_time;
        }
        //Time calculation
        current_time += processing[i].burst_time;
        calculate_times(&processing[i], current_time);
        printf("| %s %d ", processing[i].id, current_time);
    }

    printf("|\n");
    display(processing, n);
}

//Priority based Preempcve (PP)
void preemptive_priority(Processing processing[], int n) {
    int completed = 0, current_time = 0;
    printf("\nGantt Chart[Priority based Preempcve(PP)]:\n");

    while (completed < n) {
        int min_priority = 1000, selected = -1;
        for (int i = 0; i < n; i++) {
            if (!processing[i].completed &&
                processing[i].arrival_time <= current_time &&
                processing[i].priority < min_priority) {
                min_priority = processing[i].priority;
                selected = i;
            }
        }
        if (selected == -1) {
            printf("| Idle %d ", current_time + 1);
            current_time++;
            continue;
        }
        printf("| %s %d ", processing[selected].id, current_time + 1);
        
        processing[selected].remaining_time--;
        current_time++;
        //Time calculation
        if (processing[selected].remaining_time == 0) {
            processing[selected].completed = true;
            completed++;
            calculate_times(&processing[selected], current_time);
        }
    }

    printf("|\n");
    display(processing, n);
}

//Shortest Remaining Time First (SRTF)
void srtf(Processing processing[], int n) {
    int completed = 0, current_time = 0;
    printf("\nGantt Chart[Shortest Remaining Time First (SRTF)]:\n");

    while (completed < n) {
        int shortest = -1, min_remaining_time = 1000;

        for (int i = 0; i < n; i++) {
            if (!processing[i].completed &&
                processing[i].arrival_time <= current_time &&
                processing[i].remaining_time < min_remaining_time) {
                min_remaining_time = processing[i].remaining_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            printf("| Idle %d ", current_time + 1);
            current_time++;
            continue;
        }

        printf("| %s %d ", processing[shortest].id, current_time + 1);
        processing[shortest].remaining_time--;
        current_time++;
        //Time calculation
        if (processing[shortest].remaining_time == 0) {
            processing[shortest].completed = true;
            completed++;
            calculate_times(&processing[shortest], current_time);
        }
    }

    printf("|\n");
    display(processing, n);
}

//Round Robin (RR)
void round_robin(Processing processing[], int n, int time_quantum) {
    int current_time = 0, completed = 0;
    printf("\nGantt Chart[Round Robin (RR)]:\n");

    while (completed < n) {
        bool progress = false;

        for (int i = 0; i < n; i++) {
            if (processing[i].remaining_time > 0 && processing[i].arrival_time <= current_time) {
                progress = true;
                printf("| %s %d ", processing[i].id, current_time + time_quantum);
                //Time calculation
                if (processing[i].remaining_time <= time_quantum) {
                    current_time += processing[i].remaining_time;
                    processing[i].remaining_time = 0;
                    processing[i].completed = true;
                    calculate_times(&processing[i], current_time);
                    completed++;
                } else {
                    current_time += time_quantum;
                    processing[i].remaining_time -= time_quantum;
                }
            }
        }

        if (!progress) {
            printf("| Idle %d ", current_time + 1);
            current_time++;
        }
    }

    printf("|\n");
    display(processing, n);
}

//Main function
int main() {
    Processing processing[] = {
        {"P1", 0, 5, 3, 0, 0, 0, 0, false},
        {"P2", 1, 6, 5, 0, 0, 0, 0, false},
        {"P3", 3, 2, 2, 0, 0, 0, 0, false},
        {"P4", 9, 4, 1, 0, 0, 0, 0, false},
        {"P5", 12, 3, 4, 0, 0, 0, 0, false}
    };

    int n = sizeof(processing) / sizeof(processing[0]);

    sort_by_arrival(processing, n);
    fcfs(processing, n);

    reset_remaining_time(processing, n);
    preemptive_priority(processing, n);

    reset_remaining_time(processing, n);
    srtf(processing, n);

    reset_remaining_time(processing, n);
    round_robin(processing, n, 2);

    return 0;
}
