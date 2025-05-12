#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <vector>
#include "schedulealgorithms.cpp"
#include "process.h"

// Enumeración para los tipos de algoritmos de planificación
enum class ScheduleType {
    FCFS,
    SJF,
    RR,
    PRIORITY,
    REALTIME
};

// Clase Scheduler
class Scheduler {
    private:
        ScheduleAlgorithm* algorithm = nullptr; // Puntero al algoritmo de planificación
        std::queue<Process> schedule_queue;     // Cola de procesos planificados
        int queue_max_length = 0;               // Longitud máxima de la cola
        

    public:
        int quantum = 0;                        // Quantum para RR
        // Constructores
        Scheduler();
        Scheduler(ScheduleType type, int q_length, int quantum = 3);

        // Método para calendarizar procesos
        std::queue<Process> schedule(std::vector<Process>* dataProcess);

        // Destructor
        ~Scheduler();
        
};

#endif // SCHEDULER_H