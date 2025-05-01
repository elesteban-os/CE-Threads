#include <queue>
#include "process.cpp"
#include <vector>
#include <algorithm>

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

// Algoritmo SJF
class SJF : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process> dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::sort(dataProcess.begin(), dataProcess.end(), [](const Process& a, const Process& b) {
            return a.getBurstTime() < b.getBurstTime();
        });
        for (Process p : dataProcess) {
            if (q.size() == queue_length) {
                return q;
            }
            q.push(p);
        }
        return q;
    }
};

class Priority : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process> dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::sort(dataProcess.begin(), dataProcess.end(), [](const Process& a, const Process& b) {
            return a.getPriority() < b.getPriority();
        });
        for (Process p : dataProcess) {
            if (q.size() == queue_length) {
                return q;
            }
            q.push(p);
        }
        return q;
    }
};

// Algoritmo Round-Robin
class RR : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process> dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::queue<Process> tmp;

        for (Process& p : dataProcess) {
            tmp.push(p);
        }

        while (!tmp.empty()) {
            if (q.size() == queue_length) {
                return q;
            }
            
            Process current = tmp.front();
            tmp.pop();

            int remaining_time = std::max(0, current.getRemainingTime() - quantum);
            current.setRemainingTime(remaining_time);

            q.push(current);

            if (remaining_time > 0) {
                tmp.push(current);
            }
        }

        return q;
    }
};