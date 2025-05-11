


class Process {
    private:
        int process_ID;
        int burst_time;                     
        int remaining_time = burst_time;    // Para RR
        int priority;                       // Para Priority
        int max_time;                       // Para RealTime
        int deadline;                       // Para RealTime
        int remaining_deadline;             // Para RealTime
        int period;                         // Para RealTime

        Process(int process_ID, int burst_time = 0, int priority = 0, int max_time = 0, int deadline = 0, int period = 0) {
            this->process_ID = process_ID;
            this->burst_time = burst_time;
            this->priority = priority;
            
            this->max_time = max_time;
            this->deadline = deadline;
            this->period = period;

            this->remaining_time = ((burst_time != 0) ? burst_time : max_time);
            this->remaining_deadline = deadline;
        }

    public:     
        static Process withBurstTime(int process_ID, int burst_time) {
            return Process(process_ID, burst_time);
        }

        static Process withPriority(int process_ID, int priority) {
            return Process(process_ID, 0, priority);
        }

        static Process withMaxTime(int process_ID, int max_time) {
            return Process(process_ID, 0, 0, max_time);
        }

        static Process withMaxTime_Deadline_Period(int process_ID, int max_time, int deadline, int period) {
            return Process(process_ID, 0, 0, max_time, deadline, period);
        }

        // Getters
        int getProcessID() const {
            return process_ID;
        }

        int getBurstTime() const {
            return burst_time;
        }

        int getRemainingTime() const {
            return remaining_time;
        }

        int getPriority() const {
            return priority;
        }

        int getMaxTime() const {
            return max_time;
        }

        int getDeadline() const {
            return deadline;
        }

        int getPeriod() const {
            return period;
        }

        int getRemainingDeadline() const {
            return remaining_deadline;
        }

        // Setters
        void setProcessID(int id) {
            process_ID = id;
        }

        void setBurstTime(int time) {
            burst_time = time;
            remaining_time = time; 
        }

        void setRemainingTime(int time) {
            remaining_time = time;
        }

        void setPriority(int prio) {
            priority = prio;
        }

        void setMaxTime(int time) {
            max_time = time;
        }

        void setDeadline(int time) {
            deadline = time;
        }

        void setPeriod(int time) {
            period = time;
        }

        void setRemainingDeadline(int time) {
            remaining_deadline = time;
        }

        // Operator==
        bool operator==(const Process& other) const {
            return this->process_ID == other.process_ID;
        }
};