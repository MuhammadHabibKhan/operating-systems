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
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "PID" << "\tArrival Time" << "\tBurst Time" << "\tResponse Ratio" << "\tStart Time" << "\tWait Time" << "\tTurn Around Time" << "\tEnd Time" << endl;

        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
        int i = 0;
        for (Process p : list) {
            cout << p.pid << "\t" << p.arrivalTime << "\t\t" << p.burstTime << "\t\t" << p.responseRatio << "\t\t" << p.startTime << "\t\t" << p.waitTime << "\t\t" << p.turnAroundTime << "\t\t\t" << p.endTime << endl;
            i++;
        }
        cout << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    }

    void executeHRRN () {

        int i = 0;

        while (!processes.empty()) {

            shortestJob = processes[0];

            int itr = 0;
            auto it = processes.begin();

            for (auto p = processes.begin(); p != processes.end(); p++) {

                Process process = processes[itr];

                if (process.arrivalTime == shortestJob.arrivalTime) {
                    sJobs.push_back(process);
                }

                if (process.arrivalTime < shortestJob.arrivalTime) {
                    sJobs.clear();
                    sJobs.push_back(process);
                    shortestJob = process;
                    it = p;
                }
                itr++;
            }

            if (i == 0) {
                clock = shortestJob.arrivalTime;
            }

            for (auto &p: sJobs) {
                if (p.arrivalTime < clock) {
                    p.waitTime = clock - p.arrivalTime;
                }
                else if (p.arrivalTime > clock) {
                    clock = p.arrivalTime;
                }

                p.responseRatio = (p.waitTime + p.burstTime) / p.burstTime;
            }

            Process tempJob = sJobs[0];

            for (auto p = processes.begin(); p != processes.end(); p++) {

                Process process = *p;

                if (process.responseRatio > tempJob.responseRatio) {
                    tempJob = process;
                    it = p;
                }
            }

            shortestJob = tempJob;

            // cout << "wait: " << shortestJob.waitTime;
            // cout << " | clock: " << clock << endl;

            clock = clock + shortestJob.burstTime;

            completedProcess.push_back(shortestJob);
            processes.erase(it);
            sJobs.clear();

            i++;
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

};

int main () {

    Scheduler Algorithm (10);
    cout << "--------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "Input: " << endl;
    Algorithm.printProcess(Algorithm.processes);
    Algorithm.executeHRRN();
    Algorithm.calculations();
    cout << "Output: " << endl;
    Algorithm.printProcess(Algorithm.completedProcess);

    return 0;
}
