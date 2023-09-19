#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <time.h>

using namespace std;

class Process {
    public:

    int pid;
    int arrivalTime;
    int burstTime;
    int startTime;
    int endTime;
    int waitTime;
    int turnAroundTime;
    int remainingBurstTime;
    int processorUtlization;

    Process () {};

    Process (int seed) {

        mt19937 mt(seed);
        uniform_int_distribution <int> dist(1, 20);

        int randomNum = dist(mt);
        this->arrivalTime = randomNum;
        randomNum = dist(mt);
        this->burstTime = randomNum;
        this->remainingBurstTime = this->burstTime;
        this->startTime = 0;
        this->waitTime = 0;
        this->turnAroundTime = 0;
        this->endTime = 0;
    }

};

class Scheduler {
    public:

    int clock;
    int maxProcess;
    unsigned int seed = 0;

    Process shortestJob;
    vector <Process> processes;
    vector <Process> sJobs;
    vector <Process> readyQueue;
    vector <Process> completedProcess;

    Scheduler () {};

    Scheduler (int max_process) {
        this->maxProcess = max_process;
        this->clock = 0;
        
        srand(time(0));
        randomJobGenerator();
    }

    void randomJobGenerator () {

        for (int i = 0; i < maxProcess; i++) {
            Process tempProcess(seed);
            tempProcess.pid = i;
            this->processes.push_back(tempProcess);
            seed = rand();
        }
    }

    void printProcess (vector <Process> list) {
        cout << "------------------------------------------------------------------------------------------------------------" << endl;
        cout << "PID" << "\tArrival Time" << "\tBurst Time" << "\tStart Time" << "\tWait Time" << "\tTurn Around Time" << "\tEnd Time" << endl;

        cout << "------------------------------------------------------------------------------------------------------------" << endl;
        int i = 0;
        for (Process p : list) {
            cout << p.pid << "\t" << p.arrivalTime << "\t\t" << p.burstTime << "\t\t" << p.startTime << "\t\t" << p.waitTime << "\t\t" << p.turnAroundTime << "\t\t\t" << p.endTime << endl;
            i++;
        }
        cout << endl;
        cout << "------------------------------------------------------------------------------------------------------------" << endl;
    }

    void executeSRTF () {

        while (! (processes.empty() && readyQueue.empty()) ) {

            if (!processes.empty()){

                shortestJob = processes[0];
                vector <Process>::iterator arrivalIterator = processes.begin();

                for (auto p = processes.begin(); p != processes.end(); p++) {

                    Process process = *p;

                    if (shortestJob.arrivalTime > process.arrivalTime) {
                        shortestJob = process;
                        arrivalIterator = p;
                    }

                }

                if (shortestJob.arrivalTime <= clock) {
                    readyQueue.push_back(shortestJob);
                    processes.erase(arrivalIterator);
                }

            }

            if (!readyQueue.empty()) {

                vector <Process>::iterator i = readyQueue.begin();
                Process tempJob = readyQueue[0];

                // cout << "PID | Clock" << endl;
                for (auto r = readyQueue.begin(); r != readyQueue.end(); r++) {

                    Process process = *r;

                    if (process.remainingBurstTime < tempJob.remainingBurstTime) {
                        tempJob = process;
                        i = r;
                    }
                }

                shortestJob = tempJob;

                if (shortestJob.remainingBurstTime == 0) {
                    // cout << "sjob pid (srt): " << shortestJob.pid << endl;
                    cout << endl;
                    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                    cout << "PID: " << shortestJob.pid << " finished at time " << clock << endl;
                    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
                    completedProcess.push_back(shortestJob);
                    readyQueue.erase(i);
                    clock++;
                    continue;
                }

                else {
                    int index = i - readyQueue.begin();
                    shortestJob.remainingBurstTime -= 1;
                    readyQueue[index] = shortestJob;
                    // cout << "--------------" << endl;
                    cout << "| PID: " << shortestJob.pid << " | Clock: " << clock << " |" << endl;
                    // cout << "--------------" << endl;
                    // cout << "| " << shortestJob.pid << " | "<< '\n';
                    // cout << clock;
                    clock++;
                    continue;
                }
                // cout << endl;
            }
            
            clock++;
        }

    }

    void calculations () {
        for (int i = 0; i < completedProcess.size(); i++) {
            // start time
            if (i == 0) {
                completedProcess[i].startTime = completedProcess[i].arrivalTime;
            }
            else {
                if (completedProcess[i].arrivalTime > completedProcess[i-1].endTime) {
                    completedProcess[i].startTime = completedProcess[i].arrivalTime;
                }
                else if (completedProcess[i].arrivalTime < completedProcess[i-1].endTime) {
                    completedProcess[i].startTime = completedProcess[i-1].endTime;
                }
            }
            completedProcess[i].waitTime = completedProcess[i].startTime - completedProcess[i].arrivalTime;
            completedProcess[i].turnAroundTime = completedProcess[i].waitTime + completedProcess[i].burstTime;
            completedProcess[i].endTime = completedProcess[i].turnAroundTime + completedProcess[i].arrivalTime;
        }
    }

    void calculateAverage() {

        float avgWait = 0;
        float avgTurnAround = 0;
        float avgUtlization = 0;
        float totalCPUTime = 0;
        float totalTime = 0;

        for (auto process : completedProcess) {
            avgWait += process.waitTime;
            avgTurnAround += process.turnAroundTime;
            totalCPUTime += process.burstTime;
        }
        Process lastProcess = completedProcess.back();
        
        totalTime = lastProcess.endTime;
        avgUtlization = totalCPUTime / totalTime;
        avgWait = avgWait / maxProcess;
        avgTurnAround = avgTurnAround / maxProcess;

        cout << "                                             Summary                                                        " << endl;
        cout << "------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Average Wait Time \t\t | " << avgWait << endl;
        cout << "Average Turnaround Time \t | " << avgTurnAround << endl;
        cout << "Average CPU Utilization Time \t | " << avgUtlization << endl;
        cout << "------------------------------------------------------------------------------------------------------------" << endl;

    }

};

int main () {

    Scheduler Algorithm (10);
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
    cout << "                                      SHORTEST REMAINING TIME FIRST                                         " << endl;
    cout << "------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Input: " << endl;
    Algorithm.printProcess(Algorithm.processes);
    
    Algorithm.executeSRTF();
    Algorithm.calculations();

    cout << "Output: " << endl;
    Algorithm.printProcess(Algorithm.completedProcess);

    Algorithm.calculateAverage();

    return 0;
}
