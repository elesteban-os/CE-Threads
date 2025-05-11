#include <queue>
#include "process.cpp"
#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>

// Clase abstracta para los algoritmos
class ScheduleAlgorithm {
    public:
        virtual std::queue<Process> schedule(std::vector<Process>* dataProcess, int queue_length, int quantum = 0) = 0;
        virtual ~ScheduleAlgorithm() {};
};


// Algoritmo FCFS
class FCFS : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process>* dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;

        while (q.size() != queue_length && !dataProcess->empty()) {
            q.push(dataProcess->front());
            dataProcess->erase(dataProcess->begin());
        }

        return q;
    }

};



// Algoritmo SJF
class SJF : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process>* dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::sort(dataProcess->begin(), dataProcess->end(), [](const Process& a, const Process& b) {
            return a.getBurstTime() < b.getBurstTime();
        });
        while (q.size() != queue_length && !dataProcess->empty()) {
            q.push(dataProcess->front());
            dataProcess->erase(dataProcess->begin());
        }
        return q;
    }
};



// Algoritmo Priority
class Priority : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process>* dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::sort(dataProcess->begin(), dataProcess->end(), [](const Process& a, const Process& b) {
            return a.getPriority() < b.getPriority();
        });

        while (q.size() != queue_length && !dataProcess->empty()) {
            q.push(dataProcess->front());
            dataProcess->erase(dataProcess->begin());
        }
        return q;
    }
};



// Algoritmo Round-Robin
class RR : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process>* dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::queue<Process*> tmp;
        std::queue<Process*> finished_processes;

        for (Process& p : *dataProcess) {
            tmp.push(&p);
        }

        while (!tmp.empty()) {
            if (q.size() == queue_length) {
                break;
            }
            
            Process *current = tmp.front(); 
            tmp.pop();

            int remaining_time = std::max(0, current->getRemainingTime() - quantum);
            current->setRemainingTime(remaining_time);

            q.push(*current);

            if (remaining_time > 0) {
                tmp.push(current);
            } else {
                // Agregar el proceso a los que han terminado
                finished_processes.push(current);
            }
        }

        // Eliminar los procesos terminados de dataProcess
        while (!finished_processes.empty()) {
            Process* finished = finished_processes.front();
            finished_processes.pop();

            auto it = std::find_if(dataProcess->begin(), dataProcess->end(), [&](const Process& p) {
                return p.getProcessID() == finished->getProcessID();
            });

            if (it != dataProcess->end()) {
                dataProcess->erase(it);
            }
        }

        return q;
    }
};



// Algoritmo Real-Time
class RealTime : public ScheduleAlgorithm {
    std::queue<Process> schedule(std::vector<Process> *dataProcess, int queue_length, int quantum = 0) override {
        std::queue<Process> q;
        std::vector<Process> dataProcessTmp = *dataProcess;
        std::vector<Process*> tmp;

        // Ordenar los procesos por periodos (prioridad)
        std::sort(dataProcessTmp.begin(), dataProcessTmp.end(), [](const Process& a, const Process& b) {
            return a.getPeriod() < b.getPeriod(); 
        });

        int time = 0;
        while (q.size() != queue_length) {
            for (Process& p : dataProcessTmp) {
                // Comprobar si entro un nuevo proceso en el ciclo
                if (time % p.getPeriod() == 0 && p.getRemainingTime() != 0) {
                    tmp.push_back(&p);
                    std::sort(tmp.begin(), tmp.end(), [](const Process* a, const Process* b) {
                        return a->getPeriod() < b->getPeriod(); 
                    });
                }
                
            }

            if (!tmp.empty()) {
                // Agregar el proceso actual a la cola
                Process* current = tmp.front();
                q.push(*current);

                // Restar el tiempo restante del proceso actual
                int remaining_time = tmp.front()->getRemainingTime() - 1;
                tmp.front()->setRemainingTime(remaining_time);

                // Restar el tiempo de deadline
                int remaining_deadline = tmp.front()->getRemainingDeadline() - 1;
                tmp.front()->setRemainingDeadline(remaining_deadline);

                

                // Comprobar si el proceso actual termino
                if (remaining_time == 0) {
                    // Eliminarlo de la lista de procesos temporal
                    for (auto it = dataProcessTmp.begin(); it != dataProcessTmp.end(); ++it) {
                        if (it->getRemainingTime() == 0) {
                            dataProcessTmp.erase(it);
                            break; 
                        }
                    }
                    // Eliminarlo del vector temporal
                    tmp.erase(tmp.begin());
                // Comprobar si el deadline termino
                } else if (remaining_deadline == 0) {
                    // Eliminarlo del vector temporal
                    tmp.front()->setRemainingDeadline(tmp.front()->getDeadline());
                    tmp.erase(tmp.begin());
                }

            } else {
                // Proceso IDLE (no hay nada que hacer)
                Process current = Process::withBurstTime(-1, 0);
                q.push(current);
            }
            time++;
        }

        return q;

    }
};

