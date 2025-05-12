#include "processmanagement.h"

// Implementación de ProcessData::operator==
bool ProcessData::operator==(const ProcessData& other) const {
    return direction == other.direction &&
           actualProcessIDQueue == other.actualProcessIDQueue &&
           actualProcess == other.actualProcess &&
           processesEmpty == other.processesEmpty &&
           scheduleType == other.scheduleType;
}

// Constructor de ProcessManagement
ProcessManagement::ProcessManagement(ScheduleType st, int ql, FlowAlgorithm fa, int flattr, int quantum) {
    scheduler = new Scheduler(st, ql, quantum);
    schedulerType = st;

    switch (fa) {
        case FlowAlgorithm::EQUITY:
            flow = new Equity(flattr);
            break;
        case FlowAlgorithm::SIGN:
            flow = new Sign(flattr);
            break;
        case FlowAlgorithm::FIFO:
            flow = new FIFO();
            break;
        default:
            throw std::invalid_argument("Invalid FlowAlgorithm");
    }
    flowAlgorithm = fa;
    actualScheduled = &leftScheduled;
    actualProcess = nullptr;
}

// Métodos públicos
void ProcessManagement::newRightProcess(Process process) {
    processesRight.push_back(process);
}

void ProcessManagement::newLeftProcess(Process process) {
    processesLeft.push_back(process);
}

ProcessData* ProcessManagement::getData() {
    if (!processesLeft.empty() || !processesRight.empty() || !leftScheduled.empty() || !rightScheduled.empty()) {
        // Gestión de flujo
        if ((schedulerType == ScheduleType::RR || schedulerType == ScheduleType::REALTIME)) {
            if (actualScheduled->empty()) {
                while (!flowCheck());
            }
        } else {
            flowCheck();
        }

        nextProcessChecked();

        // Crear la data que verá la UI
        ProcessData* pd = new ProcessData;
        pd->direction = flow->getSignDirection();

        std::queue<Process> tempQueue = *actualScheduled;

        while (!tempQueue.empty()) {
            Process p = tempQueue.front();
            pd->actualProcessIDQueue.push(p.getProcessID());
            tempQueue.pop();
        }
        pd->actualProcess = actualProcess;
        pd->scheduleType = schedulerType;
        pd->flowAlgorithm = flowAlgorithm;

        return pd;
    } else {
        return nullptr;
    }
}

// Métodos privados
int ProcessManagement::flowCheck() {
    if (flow->update()) {
        if (flow->getSignDirection() == SignDirection::LEFT) {
            actualScheduled = &rightScheduled;
            return 1;
        } else {
            actualScheduled = &leftScheduled;
            return 1;
        }
    }
    return 0;
}

Process* ProcessManagement::nextProcessFIFO() {
    if (rightScheduled.empty()) {
        rightScheduled = scheduler->schedule(&processesRight);
    }
    if (leftScheduled.empty()) {
        leftScheduled = scheduler->schedule(&processesLeft);
    }

    Process* lowestIDProcess = nullptr;

    if (!rightScheduled.empty() && !leftScheduled.empty()) {
        Process* rightFront = &rightScheduled.front();
        Process* leftFront = &leftScheduled.front();

        if (rightFront->getProcessID() < leftFront->getProcessID()) {
            lowestIDProcess = rightFront;
            rightScheduled.pop();
        } else {
            lowestIDProcess = leftFront;
            leftScheduled.pop();
        }
    } else if (!rightScheduled.empty()) {
        lowestIDProcess = &rightScheduled.front();
        rightScheduled.pop();
    } else if (!leftScheduled.empty()) {
        lowestIDProcess = &leftScheduled.front();
        leftScheduled.pop();
    }

    return lowestIDProcess;
}

Process* ProcessManagement::nextProcessTime() {
    if (flow->getSignDirection() == SignDirection::LEFT) {
        if (rightScheduled.empty()) {
            rightScheduled = scheduler->schedule(&processesRight);
        }
        if (!rightScheduled.empty()) {
            actualProcess = &rightScheduled.front();
            rightScheduled.pop();
        } else {
            actualProcess = nullptr;
        }
    } else {
        if (leftScheduled.empty()) {
            leftScheduled = scheduler->schedule(&processesLeft);
        }
        if (!leftScheduled.empty()) {
            actualProcess = &leftScheduled.front();
            leftScheduled.pop();
        } else {
            actualProcess = nullptr;
        }
    }
    return actualProcess;
}

Process* ProcessManagement::nextProcessEquity() {
    Process* nextProcess = nextProcessTime();
    if (nextProcess == nullptr) {
        if (flow->getSignDirection() == SignDirection::LEFT) {
            if (leftScheduled.empty()) {
                leftScheduled = scheduler->schedule(&processesLeft);
            }
            if (!leftScheduled.empty()) {
                actualProcess = &leftScheduled.front();
                leftScheduled.pop();
            } else {
                actualProcess = nullptr;
            }
        } else {
            if (rightScheduled.empty()) {
                rightScheduled = scheduler->schedule(&processesRight);
            }
            if (!rightScheduled.empty()) {
                actualProcess = &rightScheduled.front();
                rightScheduled.pop();
            } else {
                actualProcess = nullptr;
            }
        }
    }
    return actualProcess;
}

Process* ProcessManagement::nextProcessChecked() {
    switch (flowAlgorithm) {
        case FlowAlgorithm::FIFO:
            actualProcess = nextProcessFIFO();
            break;
        case FlowAlgorithm::SIGN:
            actualProcess = nextProcessTime();
            break;
        case FlowAlgorithm::EQUITY:
            actualProcess = nextProcessEquity();
            break;
        default:
            throw std::invalid_argument("Invalid FlowAlgorithm");
    }
    return actualProcess;
}