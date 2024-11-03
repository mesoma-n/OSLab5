#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"

// Function to find the waiting time for Round Robin (RR) scheduling
void findWaitingTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    int remaining_bt[n];  // Array to keep track of remaining burst times
    int t = 0;  // Current time
    
    // Initialize remaining burst times
    for (int i = 0; i < n; i++) 
        remaining_bt[i] = plist[i].bt;
    
    int processes_completed = 0;  // Counter for completed processes
    
    // Loop until all processes are completed
    while (processes_completed < n) {
        // Traverse through all processes
        for (int i = 0; i < n; i++) {
            // If the process still has burst time left
            if (remaining_bt[i] > 0) {
                if (remaining_bt[i] > quantum) {
                    // If the remaining burst time is greater than quantum
                    t += quantum;  // Increment current time by quantum
                    remaining_bt[i] -= quantum;  // Decrease the remaining burst time
                } else {
                    // If this is the last cycle for the process
                    t += remaining_bt[i];  // Increment current time by remaining burst time
                    plist[i].wt = t - plist[i].bt;  // Calculate waiting time
                    remaining_bt[i] = 0;  // Mark the process as completed
                    processes_completed++;  // Increment the completed process counter
                }
            }
        }
    }
}

// Function to find the waiting time for Shortest Job First (SJF) scheduling
void findWaitingTimeSJF(ProcessType plist[], int n) 
{ 
    int remaining_bt[n];  // Array to keep track of remaining burst times
    int finished[n];  // Array to mark processes as finished
    int current_time = 0;  // Current time
    int completed = 0;  // Counter for completed processes
    
    // Initialize remaining burst times and finished statuses
    for (int i = 0; i < n; i++) {
        remaining_bt[i] = plist[i].bt;
        finished[i] = 0;
    }
    
    // Loop until all processes are completed
    while (completed != n) {
        int min_bt_index = -1;
        int shortest_bt = INT_MAX;  // To find the process with the shortest burst time
        
        // Find the process with the shortest remaining burst time
        for (int i = 0; i < n; i++) {
            if (!finished[i] && remaining_bt[i] < shortest_bt) {
                shortest_bt = remaining_bt[i];
                min_bt_index = i;
            }
        }

        // If no process was found, increment time and continue
        if (min_bt_index == -1) {
            current_time++;
            continue;
        }

        // Process the found process with the shortest burst time
        current_time += remaining_bt[min_bt_index];  // Increment current time
        plist[min_bt_index].wt = current_time - plist[min_bt_index].bt;  // Calculate waiting time
        remaining_bt[min_bt_index] = 0;  // Mark process as completed
        finished[min_bt_index] = 1;  // Mark as finished
        completed++;  // Increment the completed process counter
    }
}

// Function to find the waiting time for First Come First Serve (FCFS) scheduling
void findWaitingTime(ProcessType plist[], int n)
{ 
    // The waiting time for the first process is 0 or its arrival time
    plist[0].wt = 0 + plist[0].art; 
  
    // Calculate waiting time for all other processes
    for (int i = 1; i < n; i++) 
        plist[i].wt = plist[i-1].bt + plist[i-1].wt; 
}

// Function to calculate turnaround time for all processes
void findTurnAroundTime(ProcessType plist[], int n)
{ 
    // Turnaround time is the sum of burst time and waiting time for each process
    for (int i = 0; i < n; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
}

// Comparator function to sort processes by priority
int my_comparer(const void *this, const void *that)
{ 
    ProcessType *procA = (ProcessType *)this;
    ProcessType *procB = (ProcessType *)that;

    // Return negative, zero, or positive based on priority comparison
    return procA->pri - procB->pri; // Sorting in ascending order by priority
}

// Function to calculate average time for First Come First Serve (FCFS)
void findavgTimeFCFS(ProcessType plist[], int n) 
{ 
    // Find waiting time using FCFS
    findWaitingTime(plist, n); 
  
    // Find turnaround time
    findTurnAroundTime(plist, n); 
  
    // Display results
    printf("\n*********\nFCFS\n");
}

// Function to calculate average time for Shortest Job First (SJF)
void findavgTimeSJF(ProcessType plist[], int n) 
{ 
    // Find waiting time using SJF
    findWaitingTimeSJF(plist, n); 
  
    // Find turnaround time
    findTurnAroundTime(plist, n); 
  
    // Display results
    printf("\n*********\nSJF\n");
}

// Function to calculate average time for Round Robin (RR)
void findavgTimeRR(ProcessType plist[], int n, int quantum) 
{ 
    // Find waiting time using RR
    findWaitingTimeRR(plist, n, quantum); 
  
    // Find turnaround time
    findTurnAroundTime(plist, n); 
  
    // Display results
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

// Function to calculate average time for Priority scheduling
void findavgTimePriority(ProcessType plist[], int n) 
{ 
    // Sort the processes according to priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);
  
    // Apply FCFS after sorting
    findWaitingTime(plist, n); 
    findTurnAroundTime(plist, n); 
  
    // Display results
    printf("\n*********\nPriority\n");
}

// Function to print metrics for processes
void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turnaround time
    for (int i = 0; i < n; i++) 
    { 
        total_wt += plist[i].wt; 
        total_tat += plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    // Calculate average waiting time and turnaround time
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
}

// Function to initialize process list from the input file
ProcessType * initProc(char *filename, int *n) 
{
    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        fflush(stdout);
        exit(0);
    }

    ProcessType *plist = parse_file(input_file, n);  // Parse processes from file
    fclose(input_file);  // Close the input file

    return plist;
}
  
// Main driver function
int main(int argc, char *argv[]) 
{ 
    int n;  // Number of processes
    int quantum = 2;  // Quantum for Round Robin scheduling
    ProcessType *proc_list;

    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }
    
    // FCFS Scheduling
    n = 0;
    proc_list = initProc(argv[1], &n);  // Initialize process list from file
    findavgTimeFCFS(proc_list, n);  // Find average time using FCFS
    printMetrics(proc_list, n);  // Print process metrics
  
    // SJF Scheduling
    n = 0;
    proc_list = initProc(argv[1], &n);  // Initialize process list from file
    findavgTimeSJF(proc_list, n);  // Find average time using SJF
    printMetrics(proc_list, n);  // Print process metrics
  
    // Priority Scheduling
    n = 0; 
    proc_list = initProc(argv[1], &n);  // Initialize process list from file
    findavgTimePriority(proc_list, n);  // Find average time using Priority scheduling
    printMetrics(proc_list, n);  // Print process metrics
    
    // Round Robin Scheduling
    n = 0;
    proc_list = initProc(argv[1], &n);  // Initialize process list from file
    findavgTimeRR(proc_list, n, quantum);  // Find average time using RR
    printMetrics(proc_list, n);  // Print process metrics
    
    return 0; 
}
