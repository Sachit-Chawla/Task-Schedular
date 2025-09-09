# Task Scheduling Simulation Program

## Overview

This program simulates various task scheduling algorithms using C. It reads task specifications from an input file (`TaskSpec.txt`), applies four different scheduling algorithms (FCFS, RR, NSJF, and PSJF), and writes the results, including waiting times and average waiting times, to an output file (`Output.txt`).

---

## Features

The program implements the following scheduling algorithms:

1. **First Come First Serve (FCFS)**: Tasks are executed in the order they arrive.
2. **Round Robin (RR)**: Tasks are executed in a circular fashion with a fixed time quantum.
3. **Non-Preemptive Shortest Job First (NSJF)**: The task with the shortest burst time is executed first, without preemption.
4. **Preemptive Shortest Job First (PSJF)**: The task with the shortest remaining time is executed, preempting the currently running task if necessary.

---

## Input and Output

### Input File (`TaskSpec.txt`)
The input file should contain task details in the following format:
```
<task_name>,<arrival_time>,<burst_time>
```
- **task_name**: Name of the task (up to 9 characters).
- **arrival_time**: Time the task arrives (integer).
- **burst_time**: Time required to complete the task (integer).

Example:
```
Task1,0,5
Task2,2,3
Task3,3,8
```

### Output File (`Output.txt`)
The output file contains the schedule for each algorithm, including:
- Start and end times of each task.
- Waiting time for each task.
- Average waiting time for all tasks.

---

## How to Use

1. **Prepare Input File**: Create a file named `TaskSpec.txt` with the task details as described above.
2. **Compile the Program**:
   ```bash
   gcc -o scheduler scheduler.c
   ```
3. **Run the Program**:
   ```bash
   ./scheduler
   ```
4. **View Results**: Open `Output.txt` to see the results.

---

## Code Explanation

1. **Structure Definition**:
   - `Task` structure holds details about each task, such as name, arrival time, burst time, and waiting time.

2. **Main Function**:
   - Reads tasks from `TaskSpec.txt`.
   - Calls scheduling functions (`scheduleFCFS`, `scheduleRR`, `scheduleNSJF`, `schedulePSJF`).
   - Outputs results to `Output.txt`.

3. **Reset Function**:
   - Resets remaining times for algorithms requiring multiple iterations (e.g., RR, PSJF).

4. **Scheduling Functions**:
   - Each function implements a specific scheduling algorithm, calculates waiting times, and logs results.

---

## Limitations
- The program assumes a maximum of 50 tasks.
- Task names are limited to 9 characters.

---
