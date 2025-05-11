
// Imports
#include <queue>
#include "schedulealgorithms.cpp"
#include <vector>
#include <iostream>

enum class ScheduleType {
    FCFS,
    SJF,
    RR,
    PRIORITY,
    REALTIME
};

class Scheduler {
    private:
        ScheduleAlgorithm* algorithm = nullptr; // Initialize to nullptr
        std::queue<Process> schedule_queue;
        int queue_max_length = 0;
        int quantum = 0; // Para RR
    
    public:
        // Constructores para definir el algoritmo
        Scheduler() {
            algorithm = new FCFS(); // Replace with a concrete subclass of FCFS
        }

        Scheduler(ScheduleType type, int q_length, int quantum = 0) {
            this->queue_max_length = q_length;
            switch (type) {
                case(ScheduleType::FCFS):
                    algorithm = new FCFS();
                    break;
                case(ScheduleType::SJF):
                    algorithm = new SJF();
                    break;
                case(ScheduleType::PRIORITY):
                    algorithm = new Priority();
                    break;
                case(ScheduleType::RR):
                    algorithm = new RR();
                    this->quantum = quantum; // Asignar quantum en RR
                    break;
                case(ScheduleType::REALTIME):
                    algorithm = new RealTime;
                    break;

                // Asignar quantum en RR
                default:
                    algorithm = new FCFS();
                    break;
            }
        }


        // Calendarizar con los procesos
        std::queue<Process> schedule(std::vector<Process> *dataProcess) {
            this->schedule_queue = this->algorithm->schedule(dataProcess, this->queue_max_length, this->quantum);
            return this->schedule_queue;
        }

};


// Probar el calendarizador
int main() {
    // Crear un scheduler
    Scheduler scheduler(ScheduleType::REALTIME, 10, 3);
    
    // Crear algunos procesos
    std::vector<Process> processes;

    // Prueba RR
    // processes.push_back(Process::withBurstTime(1, 4));
    // processes.push_back(Process::withBurstTime(2, 2));
    // processes.push_back(Process::withBurstTime(3, 12));
    // processes.push_back(Process::withBurstTime(4, 15));

    // Prueba Priority
    // processes.push_back(Process::withPriority(1, 4));
    // processes.push_back(Process::withPriority(2, 2));
    // processes.push_back(Process::withPriority(3, 20));
    // processes.push_back(Process::withPriority(4, 15));

    // Prueba SJF
    //processes.push_back(Process::withBurstTime(1, 4));
    //processes.push_back(Process::withBurstTime(2, 2));
    //processes.push_back(Process::withBurstTime(3, 20));
    //processes.push_back(Process::withBurstTime(4, 15));

    // Prueba RealTime
    processes.push_back(Process::withMaxTime_Deadline_Period(1, 20, 1, 6));
    processes.push_back(Process::withMaxTime_Deadline_Period(2, 20, 2, 9));
    processes.push_back(Process::withMaxTime_Deadline_Period(3, 20, 6, 18));
    //processes.push_back(Process::withMaxTime_Deadline_Period(4, 20, 6, 24));
    //processes.push_back(Process::withMaxTime_Deadline_Period(4, 9));
    //processes.push_back(Process::withMaxTime_Deadline_Period(5, 53));


    while (!processes.empty()) {
        // Calendarizar los procesos
        std::cout << "Schedule Iteration" << std::endl;
        std::queue<Process> scheduled_processes = scheduler.schedule(&processes);
        while (!scheduled_processes.empty()) {
            Process p = scheduled_processes.front();
            scheduled_processes.pop();
            std::cout << "Process ID: " << p.getProcessID() << ", Remaining time: " << p.getRemainingTime() << ", Deadline: " << p.getRemainingDeadline() << ", Period: " << p.getPeriod() <<std::endl;
            //std::cout << p.getProcessID() << ", "  ;
        }
    }
    
    
}

