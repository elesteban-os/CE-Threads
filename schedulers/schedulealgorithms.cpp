
#include <queue>
#include "process.cpp"
#include <vector>

// Clase abstracta para los algoritmos
class ScheduleAlgorithm {
    public:
        virtual std::queue<Process> schedule(std::vector<Process> dataProcess, int queue_length, int quantum = 0) = 0;
        virtual ~ScheduleAlgorithm() {};
};


// Algoritmo FCFS
class FCFS : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process> dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        for (Process p : dataProcess) {
            if (q.size() == queue_length) {
                return q;
            }
            q.push(p);
        }
        return q;
    }

};