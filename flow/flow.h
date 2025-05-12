#ifndef FLOW_H
#define FLOW_H

#include <iostream>
#include <chrono>

// Enumeraciones
enum class FlowAlgorithm { EQUITY, SIGN, FIFO };
enum class SignDirection { LEFT, RIGHT };

// Estructuras de datos
struct EquityData {
    int W;
    int processes_executed = 0;
};

struct SignData {
    int time;       // Tiempo que debe pasar para cambiar el letrero
    long long elapsed; 
    std::chrono::steady_clock::time_point last_execution;
};

// Clase base abstracta
class Flow {
    protected:
        FlowAlgorithm flowAlgorithm;
        SignDirection signDirection = SignDirection::RIGHT;

    public:
        Flow(FlowAlgorithm algorithm);
        virtual ~Flow();

        // Método virtual puro
        virtual int update() = 0;

        // Getters
        FlowAlgorithm getAlgorithm() const;
        SignDirection getSignDirection() const;
};

// Clase Equity
class Equity : public Flow {
    private:
        EquityData data;

    public:
        Equity(int W);
        int update() override;
        EquityData getData() const;
};

// Clase Sign
class Sign : public Flow {
    private:
        SignData data;

    public:
        Sign(int time);
        int update() override;
        SignData getData() const;
};

// Clase FIFO
class FIFO : public Flow {
    public:
        FIFO();
        int update() override;
};

#endif // FLOW_H