
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

                // Asignar quantum en RR
                default:
                    algorithm = new FCFS();
                    break;
            }
        }


        // Calendarizar con los procesos
        std::queue<Process> schedule(std::vector<Process> dataProcess) {
            this->schedule_queue = this->algorithm->schedule(dataProcess, queue_max_length, this->quantum);
            return this->schedule_queue;
        }

};


// Probar el calendarizador
int main() {
    // Crear un scheduler
    Scheduler scheduler(ScheduleType::RR, 50, 3);
    
    // Crear algunos procesos
    std::vector<Process> processes;
    processes.push_back(Process::withBurstTime(1, 18));
    processes.push_back(Process::withBurstTime(2, 13));
    processes.push_back(Process::withBurstTime(3, 24));
    processes.push_back(Process::withBurstTime(4, 9));
    processes.push_back(Process::withBurstTime(5, 53));
    
    // Calendarizar los procesos
    std::queue<Process> scheduled_processes = scheduler.schedule(processes);
    
    // Imprimir los procesos programados
    while (!scheduled_processes.empty()) {
        Process p = scheduled_processes.front();
        scheduled_processes.pop();
        std::cout << "Process ID: " << p.getProcessID() << ", Remaining time:" << p.getRemainingTime() << std::endl;
    }
    
    return 0;
}

