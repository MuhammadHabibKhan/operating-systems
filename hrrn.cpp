#include <iostream>
#include <vector>
#include <string>
#include <random>
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
    float responseRatio;
    int turnAroundTime;
    int processorUtlization;

    Process () {};

    Process (int seed) {

        mt19937 mt(seed);
        uniform_int_distribution <int> dist(1, 20);

        int randomNum = dist(mt);
        this->arrivalTime = randomNum;
        randomNum = dist(mt);
        this->burstTime = randomNum;
        this->startTime = 0;
        this->waitTime = 0;
        this->endTime = 0;
        this->responseRatio = 0;
        this->turnAroundTime = 0;
    }

};

class Scheduler {
    public:

    unsigned int seed = 0;
    int maxProcess;
    int clock;

    Process shortestJob;
    vector <Process> processes;
    vector <Process> sJobs;
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
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
        cout << "PID" << "\t Arrival Time" << "\tBurst Time" << "\tStart Time" << "\tWait Time" << "\tTurn Around Time" << "\tEnd Time" << endl;

        cout << "----------------------------------------------------------------------------------------------------------" << endl;
        int i = 0;
        for (Process p : list) {
            cout << p.pid << "\t\t" << p.arrivalTime << "\t\t" << p.burstTime << "\t\t" << p.startTime << "\t\t" << p.waitTime << "\t\t" << p.turnAroundTime << "\t\t" << p.endTime << endl;
            i++;
        }
        cout << endl;
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
    }

    void executeHRRN () {

        while ( !(processes.empty() && sJobs.empty()) ) {

            if (!processes.empty()) {

                for (auto p = processes.begin(); p != processes.end(); p++) {

                    Process process = *p;

                    if (process.arrivalTime <= clock) {
                        sJobs.push_back(process);
                    }
                }
            }

            for (auto &p: sJobs) {

                // cout << "pid: " << p.pid << " clock: " << clock << " arrival: " << p.arrivalTime << endl;
                if (p.arrivalTime <= clock) {
                    p.waitTime = clock - p.arrivalTime;
                }
                // cout << "W: " << p.waitTime << " B: " << p.burstTime << endl;
                 p.responseRatio = (p.waitTime + p.burstTime) / p.burstTime;
                //  cout << "RR: " << p.responseRatio << endl;
            }

            if (!sJobs.empty()) {

                Process tempJob = sJobs[0];

                cout << "----------------------------------------------------" << endl;
                cout << "Ready Queue at time " << clock << " : " << endl;

                for (auto p = sJobs.begin(); p != sJobs.end(); p++) {

                    Process process = *p;

                    cout << "PID: " << process.pid << " | " << "RR: " << process.responseRatio << " |";
                    cout << endl;

                    if (process.responseRatio > tempJob.responseRatio) {
                        tempJob = process;
                    }
                }
                cout << "----------------------------------------------------" << endl;
                cout << "PID with highest reponse ratio selected: " << tempJob.pid << endl;
                cout << "----------------------------------------------------" << endl;

                shortestJob = tempJob;

                int s_pid = shortestJob.pid;

                for (auto p = processes.begin(); p != processes.end(); p++) {

                    Process pr = *p;

                    if (pr.pid == s_pid) {
                        // cout << pr.pid << " erased " << endl;
                        processes.erase(p);
                        break;
                    }
                }

                completedProcess.push_back(shortestJob);
                sJobs.clear();
                clock = clock + shortestJob.burstTime;
                continue;
            }
            cout << "++" << endl;
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

        cout << "                                             Summary                                                      " << endl;
        cout << "----------------------------------------------------------------------------------------------------------" << endl;
        cout << "Average Wait Time \t\t | " << avgWait << endl;
        cout << "Average Turnaround Time \t | " << avgTurnAround << endl;
        cout << "Average CPU Utilization Time \t | " << avgUtlization << endl;
        cout << "----------------------------------------------------------------------------------------------------------" << endl;

    }

};

int main () {

    Scheduler Algorithm (10);
    cout << "----------------------------------------------------------------------------------------------------------" << endl;
    cout << "                                            HIGHEST RESPONSE RATIO NEXT                                   " << endl;
    cout << "----------------------------------------------------------------------------------------------------------" << endl;
    cout << "Input: " << endl;
    
    Algorithm.printProcess(Algorithm.processes);
    
    Algorithm.executeHRRN();
    Algorithm.calculations();
    
    cout << "Output: " << endl;
    Algorithm.printProcess(Algorithm.completedProcess);

    Algorithm.calculateAverage();

    return 0;
}
