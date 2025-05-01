class Process {
    private:
        int process_ID;
        int burst_time;
        int remaining_time = burst_time;    // Para RR
        int priority;                       // Para Priority
        int max_time;                       // Para RealTime

        Process(int process_ID, int burst_time = 0, int priority = 0, int max_time = 0) {
            this->process_ID = process_ID;
            this->burst_time = burst_time;
            this->priority = priority;
            this->remaining_time = burst_time;
            this->max_time = max_time;
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
};