
// Imports
#include <queue>
#include "schedulers/schedulealgorithms.cpp"
#include "process.cpp"

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
                // Agregar otros 

                // Asignar quantum en RR
                default:
                    algorithm = new FCFS();
                    break;
            }
        }

        


        // Calendarizar con los procesos
        std::queue<Process> schedule(std::vector<Process> dataProcess) {
            this->schedule_queue= this->algorithm->schedule(dataProcess, queue_max_length);
            return this->schedule_queue;
        }

};