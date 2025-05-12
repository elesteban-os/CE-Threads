#ifndef PROCESS_H
#define PROCESS_H

class Process {
    private:
        int process_ID;
        int burst_time;                     
        int remaining_time;    // Para RR
        int priority;          // Para Priority
        int max_time;          // Para RealTime
        int deadline;          // Para RealTime
        int remaining_deadline; // Para RealTime
        int period;            // Para RealTime

    public:
        // Constructor
        Process(int process_ID, int burst_time = 0, int priority = 0, int max_time = 0, int deadline = 0, int period = 0);

        // Métodos estáticos para crear procesos con diferentes configuraciones
        static Process withBurstTime(int process_ID, int burst_time);
        static Process withPriority(int process_ID, int priority);
        static Process withMaxTime(int process_ID, int max_time);
        static Process withMaxTime_Deadline_Period(int process_ID, int max_time, int deadline, int period);

        // Getters
        int getProcessID() const;
        int getBurstTime() const;
        int getRemainingTime() const;
        int getPriority() const;
        int getMaxTime() const;
        int getDeadline() const;
        int getPeriod() const;
        int getRemainingDeadline() const;

        // Setters
        void setProcessID(int id);
        void setBurstTime(int time);
        void setRemainingTime(int time);
        void setPriority(int prio);
        void setMaxTime(int time);
        void setDeadline(int time);
        void setPeriod(int time);
        void setRemainingDeadline(int time);

        // Operador ==
        bool operator==(const Process& other) const;
};

#endif // PROCESS_H