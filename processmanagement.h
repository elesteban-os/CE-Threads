#ifndef PROCESSMANAGEMENT_H
#define PROCESSMANAGEMENT_H

#include "flow/flow.h"
#include "schedulers/scheduler.h"
#include <queue>
#include <vector>
#include <stdexcept>

// Estructura para datos del proceso
struct ProcessData {
    SignDirection direction;
    std::queue<int> actualProcessIDQueue;
    
    bool processesEmpty;
    ScheduleType scheduleType;
    FlowAlgorithm flowAlgorithm;

    Process* actualProcess;
    int time;  // Tiempo en el movimiento 
    int street_percentage; // Porcentaje de la calle que se va a mover



    bool operator==(const ProcessData& other) const;
};

// Clase ProcessManagement
class ProcessManagement {
    private:
        Scheduler* scheduler;
        ScheduleType schedulerType;
        Flow* flow;
        FlowAlgorithm flowAlgorithm;
        std::vector<Process> processesLeft;
        std::vector<Process> processesRight;
        std::queue<Process> leftScheduled;
        std::queue<Process> rightScheduled;
        std::queue<Process>* actualScheduled; 
        Process* actualProcess;
        bool equityLaneChanged = false;
        int street;

        // Métodos privados
        int flowCheck();
        Process* nextProcessFIFO();
        Process* nextProcessTime();
        Process* nextProcessEquity();
        Process* nextProcessChecked();
        Process* nextProcessSign(SignDirection sd);

    public:
        // Constructor
        ProcessManagement(ScheduleType st, int ql, FlowAlgorithm fa, int flattr, int quantum = 3, int sttr = 0);

        // Métodos públicos
        void newRightProcess(Process process);
        void newLeftProcess(Process process);
        ProcessData* getData();
};

#endif // PROCESSMANAGEMENT_H