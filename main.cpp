#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <string>

using namespace std;

struct process
{
    int process_num;
    int initial_burst_time;
    int left_burst_time;
    int finished_at = 0; // tells which queue process finished at, either 1, 2, 3.
};

void process_insertion(queue<process> &q, vector<vector<string>> &movement_log)
{
    srand(time(0));
    for(int i=0; i<100; i++) // creating 100 processes
    {
        process p;
        p.process_num = i + 1;
        p.initial_burst_time = rand()%100 + 1; // each process gets a random burst time ranging from 1 to 100 ms
        p.left_burst_time = p.initial_burst_time;
        q.push(p); // each process is inserted in the first waiting queue

        vector<string> v;
        movement_log.push_back(v);
    }
    return;
}

int main()
{
    vector<process> finished_processes; // tracks processes at the end with their order of termination
    vector<vector<string>> movement_log; // tracks movement history of each process
    queue<process> queue_1;
    queue<process> queue_2;
    queue<process> queue_3;
    queue<process> waiting_1; // waiting queue for processes bound to enter queue 1
    queue<process> waiting_2; // waiting queue for processes bound to enter queue 2
    queue<process> waiting_3; // waiting queue for processes bound to enter queue 3

    // first and second queue use Round Robin algorithm, third queue uses First Come First Served algorithm
    int q1_max_size = 10;
    int q2_max_size = 20;
    int q3_max_size = 30;
    int q1_time_quantum = 8;
    int q2_time_quantum = 16;

    int remaining_q1_time_quantum = q1_time_quantum;
    int remaining_q2_time_quantum = q2_time_quantum;

    string message;

    process_insertion(waiting_1, movement_log); // creates 100 processes and stores them in first waiting queue

    for(int i=0; i<q1_max_size; i++) // pops 10 processes from first waiting queue and pushes them in queue 1
    {
        process p = waiting_1.front();
        waiting_1.pop();
        queue_1.push(p);
    }

    cout<<"Log of process remaining time and movement:"<<endl;
    cout<<"-------------------------------------------"<<endl;

    // used approach in dividing CPU time among queues is as follows:
    // 5 ms for queue 1
    // 3 ms for queue 2
    // 2 ms for queue 3
    // in order to achieve 50% CPU time for queue 1, 30% CPU time for queue 2, 20% CPU time for queue 3

    // main loop works as long as there is a process left in any of the three queues or waiting queues
    while(!queue_1.empty() || !queue_2.empty() || !queue_3.empty() || !waiting_1.empty() || !waiting_2.empty() || !waiting_3.empty())
    {
        if(!queue_1.empty() || !waiting_1.empty()) // this section handles functionality of queue 1
        {
            for(int i=0; i<5 && queue_1.size() > 0; i++) // simulates 5 ms assigned to queue 1
            {
                process &p = queue_1.front();
                p.left_burst_time -= 1;
                remaining_q1_time_quantum -= 1;

                if(remaining_q1_time_quantum == 0) // process either finished and terminates, or is headed to queue 2
                {
                    queue_1.pop();
                    remaining_q1_time_quantum = q1_time_quantum;

                    if(p.left_burst_time <= 0) // checks if process finished execution
                    {
                        message = "->  process " + to_string(p.process_num) + " from queue 1 finished at 1";
                        movement_log[p.process_num -1].push_back(message);
                        cout<<message<<endl;

                        p.finished_at = 1;
                        finished_processes.push_back(p);
                    }
                    else // process is inserted to second waiting queue
                    {
                        message = "->  process " + to_string(p.process_num) + " from queue 1 to waiting 2";
                        movement_log[p.process_num -1].push_back(message);
                        cout<<message<<endl;

                        waiting_2.push(p);
                    }
                }
            }

            // pops processes out of first waiting queue and pushes them into queue 1 if there is free space
            if(queue_1.size() <  q1_max_size && !waiting_1.empty())
            {
                while(queue_1.size() <  q1_max_size && !waiting_1.empty())
                {
                    process &new_p = waiting_1.front();
                    waiting_1.pop();
                    queue_1.push(new_p);

                    message = "->  process " + to_string(new_p.process_num) + " from waiting 1 to queue 1";
                    movement_log[new_p.process_num -1].push_back(message);
                    cout<<message<<endl;
                }
            }
        }

        if(!queue_2.empty() || !waiting_2.empty()) // this section handles functionality of queue 2
        {
            for(int i=0; i<3 && queue_2.size() > 0; i++) // simulates 3 ms assigned to queue 2
            {
                process &p = queue_2.front();
                p.left_burst_time -= 1;
                remaining_q2_time_quantum -= 1;

                if(remaining_q2_time_quantum == 0) // process either finished and terminates, is headed to queue 1, or is headed to queue 3
                {
                    queue_2.pop();
                    remaining_q2_time_quantum = q2_time_quantum;

                    if(p.left_burst_time <= 0) // checks if process finished execution
                    {
                        message = "->  process " + to_string(p.process_num) + " from queue 2 finished at 2";
                        movement_log[p.process_num -1].push_back(message);
                        cout<<message<<endl;

                        p.finished_at = 2;
                        finished_processes.push_back(p);
                    }
                    else // process is randomly inserted to either the first waiting queue, or second waiting queue
                    {
                        int random = rand() % 2;
                        if(random == 0)
                        {
                            message = "->  process " + to_string(p.process_num) + " from queue 2 to waiting 3";
                            movement_log[p.process_num -1].push_back(message);
                            cout<<message<<endl;

                            waiting_3.push(p);
                        }
                        else
                        {
                            message = "->  process " + to_string(p.process_num) + " from queue 2 to waiting 1";
                            movement_log[p.process_num -1].push_back(message);
                            cout<<message<<endl;

                            waiting_1.push(p);
                        }
                    }
                }
            }

            // pops processes out of second waiting queue and pushes them into queue 2 if there is space
            if(queue_2.size() < q2_max_size && !waiting_2.empty())
            {
                while(queue_2.size() < q2_max_size && !waiting_2.empty())
                {
                    process &new_p = waiting_2.front();
                    waiting_2.pop();
                    queue_2.push(new_p);

                    message = "->  process " + to_string(new_p.process_num) + " from waiting 2 to queue 2";
                    movement_log[new_p.process_num -1].push_back(message);
                    cout<<message<<endl;
                }
            }
        }
        if(!queue_3.empty() || !waiting_3.empty()) // this section handles functionality of queue 3
        {
            for(int i=0; i<2 && queue_3.size() > 0; i++) // simulates 2 ms assigned to queue 3
            {
                process &p = queue_3.front();
                p.left_burst_time -= 1;

                if(p.left_burst_time <= 0) // checks if process finished execution
                {
                    queue_3.pop();

                    message = "->  process " + to_string(p.process_num) + " from queue 3 finished at 3";
                    movement_log[p.process_num -1].push_back(message);
                    cout<<message<<endl;

                    p.left_burst_time = 0;
                    p.finished_at = 3;
                    finished_processes.push_back(p);
                }
            }

            // pops processes out of third waiting queue and pushes them into queue 3 if there is space
            if(queue_3.size() < q3_max_size && !waiting_3.empty())
            {
                while(queue_3.size() < q3_max_size && !waiting_3.empty())
                {
                    process &new_p = waiting_3.front();
                    waiting_3.pop();
                    queue_3.push(new_p);

                    message = "->  process " + to_string(new_p.process_num) + " from waiting 3 to queue 3";
                    movement_log[new_p.process_num -1].push_back(message);
                    cout<<message<<endl;
                }
            }
        }
    }

    cout<<endl<<endl;
    cout<<"Processes in order of completed execution:"<<endl;
    cout<<"------------------------------------------"<<endl;

    for(int i=0; i<finished_processes.size(); i++)
    {
        process p = finished_processes[i];
        cout<<i+1<<"- p"<<p.process_num<<" with burst time: "<<p.initial_burst_time<<" ms exited from queue: "<<p.finished_at<<endl;
    }

    // the following code allows user to trace movement history of individual processes
    Label:
    cout<<endl<<endl;
    int p_num;
    cout<<"Enter number of process to trace movement, or -1 to exit: ";
    cin>>p_num;

    if(p_num == -1)
        return 0;

    if(p_num < 1 || p_num > 100)
    {
        cout<<"Enter a suitable number. Processes' numbers are from 1 to 100.";
        goto Label;
    }

    process p;
    for(int i=0; i<finished_processes.size(); i++)
    {
        if(finished_processes[i].process_num == p_num)
            p = finished_processes[i];
    }
    cout<<"Process required burst time: "<<p.initial_burst_time<<" ms"<<endl;
    for(int i=0; i<movement_log[p_num - 1].size(); i++)
    cout<<movement_log[p_num - 1][i]<<endl;
    goto Label;

    return 0;
}
