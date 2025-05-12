#include "process.h"

// Constructor
Process::Process(int process_ID, int burst_time, int priority, int max_time, int deadline, int period)
    : process_ID(process_ID),
      burst_time(burst_time),
      remaining_time_RR(burst_time),
      remaining_time(max_time),
      priority(priority),
      max_time(max_time),
      deadline(deadline),
      remaining_deadline(deadline),
      period(period) {}

// Métodos estáticos
Process Process::withBurstTime(int process_ID, int burst_time) {
    return Process(process_ID, burst_time);
}

Process Process::withPriority(int process_ID, int priority) {
    return Process(process_ID, 0, priority);
}

Process Process::withMaxTime(int process_ID, int max_time) {
    return Process(process_ID, 0, 0, max_time);
}

Process Process::withMaxTime_Deadline_Period(int process_ID, int max_time, int deadline, int period) {
    return Process(process_ID, 0, 0, max_time, deadline, period);
}

// Getters
int Process::getProcessID() const {
    return process_ID;
}

int Process::getBurstTime() const {
    return burst_time;
}

int Process::getRemainingTime() const {
    return remaining_time;
}

int Process::getPriority() const {
    return priority;
}

int Process::getMaxTime() const {
    return max_time;
}

int Process::getDeadline() const {
    return deadline;
}

int Process::getPeriod() const {
    return period;
}

int Process::getRemainingDeadline() const {
    return remaining_deadline;
}

int Process::getRemainingTime_RR() const {
    return remaining_time_RR;
}

// Setters
void Process::setProcessID(int id) {
    process_ID = id;
}

void Process::setBurstTime(int time) {
    burst_time = time;
    remaining_time = time; 
}

void Process::setRemainingTime(int time) {
    remaining_time = time;
}

void Process::setPriority(int prio) {
    priority = prio;
}

void Process::setMaxTime(int time) {
    max_time = time;
}

void Process::setDeadline(int time) {
    deadline = time;
}

void Process::setPeriod(int time) {
    period = time;
}

void Process::setRemainingDeadline(int time) {
    remaining_deadline = time;
}


void Process::setRemainingTime_RR(int time) {
    remaining_time_RR = time;
}

// Operador ==
bool Process::operator==(const Process& other) const {
    return this->process_ID == other.process_ID;
}