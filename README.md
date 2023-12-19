# Multilevel-feedback-queue
A multilevel feedback queue is a scheduling algorithm used in computer operating systems to manage the execution of processes. It operates by employing multiple queues with varying priorities to schedule processes for execution. Each queue is assigned a different priority level, and processes are placed in the queue based on their characteristics, such as runtime behavior, CPU burst times, or other criteria.

## Implemented structure
The implemented multilevel feedback queue consists of three queues.

* ### Q1
uses Round Robin algorithm
* max size: 10 processes
* time quantum: 8 ms

* ### Q2
uses Round Robin algorithm
* max size: 20 processes
* time quantum: 16 ms  

* ### Q3
uses First Come First Served algorithm
* max size: 30 processes

### Waiting queues
Each of the queues mentioned above has a designated waiting queue that stores processes bound to enter the queue but are waiting for space to be freed. 

### CPU simulated behavior
The simulated CPU divides its service time among the three queues in the following manner:
* 50% in serving Q1
* 30% in serving Q2
* 20% in serving Q3

## What the given code does
100 random processes are created, each having random CPU burst time ranging from 1 to 100 ms. All of the processes are inserted in the first waiting queue and are then fed into Q1.

If a process is fully executed after using the Q1 time quantum, it is teminated. If it still requires more CPU time, it is sent to the second waiting queue to be fed into Q2.

If a process is fully executed after using the Q2 time quantum, it is teminated. If it still requires more CPU time, the following two cases appear:
* Either it is sent to the first waiting queue to be fed into Q1 again.
* Or it is sent to the third waiting queue to be fed into Q3.

Any process that enters Q3 will finish execution there and be terminated.

## Output
### First
A movement log is printed out showing the movement of processes from source to destination. The records are printed out in the order which the CPU performs.
### Second
All processes are printed out in their order of termination, with their CPU burst time and the queue which they terminated at.
### Third
A query section is provided were user enters process number, and is given the history of movement of this process from start of the program until its termination.
