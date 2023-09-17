// 3 stages ==> Process Identification | Processes State Info | Process Control Info

// PI => Unique ID
// processes = 2
// quantum size = 2
// Process ID
// Time 
// arrival based on position in array
// execution time provided in array
// PC
// IR 
// Processed Instructions
// Resume

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// int quantumSize;
int PC = 0;
int PC_index;

class process{
    public:
        int ID;
        int IR;
        string state = "Running";
        int execTime; // execTime translates to size of vector processes that tells us the number of instructions 
        int resume = 0;
        int resumeInstruction;
        bool isComplete = false;
        bool resourceRequired;
        
        vector <int> instructions;
        vector <int> processedInstructions;
        vector <int> remainingInstructions;

    process () {};

    process(int id){
        this->ID = id;
        
        cout << "Enter number of instructions / execution time for Process ID '" << id << "' : ";
        cin >> this->execTime;
        cout << endl;

        cout << "Resource Required? : ";
        cin >> this->resourceRequired;
        cout << endl;

        for (int itr = 0; itr < execTime; itr++){
            instructions.push_back(ID+itr+1);
        }
    };
};

class pcb {
    public:
        vector <process*> processes;
        vector <process*> blockQueue;
        int id = 100;
        int quantumSize;

        pcb() {};

        pcb(int totalProcess) {
            cout << "Enter Quantum Size: ";
            cin >> this->quantumSize;
            cout << endl;

            for (int i=0; i<totalProcess; i++) {
                process* newProcess = new process(id);
                processes.push_back(newProcess);
                id += 100;
            }
        };

        void executeProcess() {
            bool x = true;
            int completeCounter = 0;

            while (x) {
                for (int i = 0; i < processes.size(); i++) {
        
                    process* currentProcess = processes[i];

                    if (!blockQueue.empty()){
                        process* currentProcess = blockQueue[0];
                        currentProcess->state = "Running";
                        currentProcess->resourceRequired = false;
                    }

                    if (!currentProcess->isComplete) {
                        cout << " --------------------------------------------" << endl;
                        cout << "Process ID: " << currentProcess->ID << endl;

                        if (currentProcess->resourceRequired){
                            currentProcess->state = "Blocked";
                            cout << "State: " << currentProcess->state << endl;
                            blockQueue.push_back(currentProcess);
                            continue;
                        }

                        cout << "State: " << currentProcess->state << endl;

                        cout << "Current Instructions: ";
                        for (auto itr : currentProcess->instructions) {
                            cout << itr << " | ";
                        }
                        cout << endl;

                        currentProcess->IR = 0; // Set IR to 0 since you're processing instructions from the beginning
                        cout << "IR: " << currentProcess->instructions[currentProcess->IR] << endl;

                        for (int x = 0; x < quantumSize && !currentProcess->instructions.empty(); x++) {
                            currentProcess->processedInstructions.push_back(currentProcess->instructions[0]);
                            currentProcess->instructions.erase(currentProcess->instructions.begin());
                        }

                        if (currentProcess->instructions.empty()) {
                            currentProcess->isComplete = true;
                        }

                        cout << "Processed Instructions: ";
                        for (auto ins: currentProcess->processedInstructions) {
                            cout << ins << " | ";
                        }
                        cout << endl;

                        currentProcess->resumeInstruction = currentProcess->instructions[currentProcess->resume];
                        if (currentProcess->isComplete) {
                            cout << "Resume Instruction: -" << endl;
                        } 
                        else {
                            cout << "Resume Instruction: " << currentProcess->resumeInstruction << endl;
                        }

                        if (!blockQueue.empty()) {
                            blockQueue.erase(blockQueue.begin());
                        }

                        PC = processes[(i+1) % (processes.size()-1)]->resume; // always resumes at 0th index as instructions are removed
                        int PC_val = processes[(i+1) % (processes.size())]->instructions[PC];

                        if (processes[(i+1) % (processes.size())]->isComplete) {
                            cout << "PC: " << " - " << endl;
                        }

                        else{
                            cout << "PC: " << PC_val << endl;
                        }

                    } else {
                        completeCounter++;
                    }

                    if (completeCounter == processes.size()) {
                        x = false;
                        break;
                    }
                }
            }

            // Deallocate memory for each process object
            for (process* p : processes) {
                delete p;
            }
            for (process* p : blockQueue) {
                delete p;
            }
        }
};

int main() {
    pcb p(4);
    p.executeProcess();

    return 0;
}
