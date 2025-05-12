#include "scheduler.h"
#include <iostream>

// Constructor por defecto
Scheduler::Scheduler() {
    algorithm = new FCFS(); // Por defecto, usar FCFS
}

// Constructor con tipo de algoritmo
Scheduler::Scheduler(ScheduleType type, int q_length, int quantum) {
    this->queue_max_length = q_length;
    switch (type) {
        case ScheduleType::FCFS:
            algorithm = new FCFS();
            break;
        case ScheduleType::SJF:
            algorithm = new SJF();
            break;
        case ScheduleType::PRIORITY:
            algorithm = new Priority();
            break;
        case ScheduleType::RR:
            algorithm = new RR();
            this->quantum = quantum; // Asignar quantum en RR
            break;
        case ScheduleType::REALTIME:
            algorithm = new RealTime();
            break;
        default:
            algorithm = new FCFS(); // Por defecto, usar FCFS
            break;
    }
}

// Método para calendarizar procesos
std::queue<Process> Scheduler::schedule(std::vector<Process>* dataProcess) {
    this->schedule_queue = this->algorithm->schedule(dataProcess, this->queue_max_length, this->quantum);
    return this->schedule_queue;
}

// Destructor
Scheduler::~Scheduler() {
    delete algorithm; // Liberar memoria del algoritmo
}