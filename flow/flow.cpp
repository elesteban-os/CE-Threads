
#include <iostream>
#include <chrono>
#include <thread>

enum class FlowAlgorithm { EQUITY, SIGN, FIFO };
enum class SignDirection { LEFT, RIGHT };

struct EquityData {
    int W;
    int processes_executed = 0;
    SignDirection signDirection = SignDirection::RIGHT;
};

struct SignData {
    int time;       // Tiempo que debe pasar para cambiar el letrero
    long long elapsed; 
    std::chrono::steady_clock::time_point last_execution;
    SignDirection signDirection = SignDirection::RIGHT;
};


class Flow {
    protected:
        FlowAlgorithm flowAlgorithm;

    public:
        Flow(FlowAlgorithm algorithm) : flowAlgorithm(algorithm) {}
        virtual ~Flow() {}

        // Método virtual puro para implementar la lógica específica
        virtual int update() = 0;

        // Getter para el algoritmo
        FlowAlgorithm getAlgorithm() const {
            return flowAlgorithm;
        }
};

class Equity : public Flow {
    private:
        EquityData data;
    public:

        Equity(int W) : Flow(FlowAlgorithm::EQUITY), data{W} {}

        // Llamar unicamente cuando termino un proceso
        int update() override { 
            
            if (data.processes_executed++ == data.W) {
                // Terminaron los procesos de este lado
                data.processes_executed = 0;
                if (data.signDirection == SignDirection::RIGHT) {
                    // Cambiar la direccion del letrero
                    data.signDirection = SignDirection::LEFT;
                } else { 
                    data.signDirection = SignDirection::RIGHT;
                }
            }
            std::cout << "Procesos ejecutados: " << data.processes_executed << std::endl;
            // Retorna 1 si hubo un cambio en la direccion de la senal
            return data.processes_executed == 0;
        }

        // Getter para los datos de Equity
        EquityData getData() const {
            return data;
        }
};

class Sign : public Flow {
    private:
        SignData data;

    public:
        Sign(int time)
            : Flow(FlowAlgorithm::SIGN), data{time} {}

        // Llamar en cada iteracion para saber el estado del reloj
        int update() override {
            //std::cout << "Update" << std::endl;
            auto now = std::chrono::steady_clock::now();
            data.elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - data.last_execution).count();

            if (data.elapsed >= data.time) {
                data.last_execution = now; 
                // Cambiar la direccion del letrero
                if (data.signDirection == SignDirection::RIGHT) {
                    data.signDirection = SignDirection::LEFT;
                } else { 
                    data.signDirection = SignDirection::RIGHT;
                }
                return 1; 
            }
            return 0;
        }

        // Getter para los datos de Sign
        SignData getData() const {
            return data;
        }
};

class FIFO : public Flow {
    public:
        FIFO() : Flow(FlowAlgorithm::FIFO) {}

        // No hacer nada
        int update() override {
            return 0; // Por ahora
        }
};


void performAction() {
    std::cout << "Acción ejecutada" << std::endl;
}

int main() {

    // Prueba para Equity
    int w = 3;
    Flow* flow = new Equity(w);

    while (true) {
        
        if (flow->update()) {
            std::cout << "Senal cambiada" << std::endl;
        }  
        // Pausar la ejecución durante 1000 milisegundos
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));      
    }

    /*
    // Prueba para Time

    int tiempo = 1;             // En segundos
    Flow* flow = new Sign(tiempo);
    while (true) {
        if (flow->update()) {
            std::cout << "Senal cambiada" << std::endl;
        }        
    }
    */
}