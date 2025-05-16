#include "flow.h"
#include <thread>

// Implementación de la clase Flow
Flow::Flow(FlowAlgorithm algorithm) : flowAlgorithm(algorithm) {}
Flow::~Flow() {}

FlowAlgorithm Flow::getAlgorithm() const {
    return flowAlgorithm;
}

SignDirection Flow::getSignDirection() const {
    return signDirection;
}

void Flow::setSignDirection(SignDirection dir) {
    this->signDirection = dir;
}


// Implementación de la clase Equity
Equity::Equity(int W) : Flow(FlowAlgorithm::EQUITY), data{W} {}

int Equity::update() {
    if (data.processes_executed == data.W) {
        data.processes_executed = 0;
        if (signDirection == SignDirection::RIGHT) {
            signDirection = SignDirection::LEFT;
        } else { 
            signDirection = SignDirection::RIGHT;
        }
    }
    data.processes_executed++;
    //std::cout << "Procesos ejecutados: " << data.processes_executed << std::endl;
    return data.processes_executed == 1;
}

EquityData Equity::getData() const {
    return data;
}

// Implementación de la clase Sign
Sign::Sign(int time) : Flow(FlowAlgorithm::SIGN), data{time} {}

int Sign::update() {
    auto now = std::chrono::steady_clock::now();
    data.elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - data.last_execution).count();

    if (data.elapsed >= data.time) {
        data.last_execution = now; 
        if (signDirection == SignDirection::RIGHT) {
            signDirection = SignDirection::LEFT;
        } else { 
            signDirection = SignDirection::RIGHT;
        }
        return 1; 
    }
    return 0;
}

SignData Sign::getData() const {
    return data;
}


// Implementación de la clase FIFO
FIFO::FIFO() : Flow(FlowAlgorithm::FIFO) {}

int FIFO::update() {
    return 1; // Por ahora
}

