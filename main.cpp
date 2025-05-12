#include "flow/flow.h"
#include "schedulers/scheduler.h"
#include "processmanagement.h"
#include <thread>


int main() {
    // Crear una instancia de ProcessManagement con:
    // - Algoritmo de planificación: SJF
    // - Longitud máxima de la cola: 5
    // - Flujo: EQUITY
    // - Atributo del flujo (W): 3
    ProcessManagement pm(ScheduleType::SJF, 3, FlowAlgorithm::EQUITY, 3);

    // // Agregar procesos al lado izquierdo
    pm.newLeftProcess(Process::withBurstTime(1, 10));
    pm.newLeftProcess(Process::withBurstTime(2, 5));
    pm.newLeftProcess(Process::withBurstTime(3, 8));

    // // Agregar procesos al lado derecho
    pm.newRightProcess(Process::withBurstTime(4, 7));
    pm.newRightProcess(Process::withBurstTime(5, 3));
    pm.newRightProcess(Process::withBurstTime(6, 12));

    // Prueba RealTime
    // pm.newLeftProcess(Process::withMaxTime_Deadline_Period(1, 20, 1, 6));
    // pm.newLeftProcess(Process::withMaxTime_Deadline_Period(2, 20, 2, 9));
    // pm.newLeftProcess(Process::withMaxTime_Deadline_Period(3, 20, 6, 18));

    // pm.newRightProcess(Process::withMaxTime_Deadline_Period(1, 20, 1, 6));
    // pm.newRightProcess(Process::withMaxTime_Deadline_Period(2, 20, 2, 9));
    // pm.newRightProcess(Process::withMaxTime_Deadline_Period(3, 20, 6, 18));
    // pm.newLeftProcess(Process::withMaxTime_Deadline_Period(4, 20, 2, 5));


    // Simular el flujo y obtener datos
    while (true) {
        ProcessData* data = pm.getData();

        if (data == nullptr) {
            std::cout << "No hay más procesos para ejecutar." << std::endl;
            break;
        }

        // Imprimir la información de los datos obtenidos
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Direccion del flujo: " 
                  << (data->direction == SignDirection::LEFT ? "Izquierda" : "Derecha") 
                  << std::endl;

        std::cout << "Procesos en la cola actual: ";
        std::queue<int> tempQueue = data->actualProcessIDQueue;
        while (!tempQueue.empty()) {
            std::cout << tempQueue.front() << " ";
            tempQueue.pop();
        }
        std::cout << std::endl;

        if (data->actualProcess != nullptr) {
            std::cout << "Proceso en ejecucion: " << data->actualProcess->getProcessID() << std::endl;
        } else {
            std::cout << "No hay proceso en ejecucion." << std::endl;
        }

        std::cout << "Algoritmo de planificacion: ";
        switch (data->scheduleType) {
            case ScheduleType::FCFS:
                std::cout << "FCFS";
                break;
            case ScheduleType::SJF:
                std::cout << "SJF";
                break;
            case ScheduleType::RR:
                std::cout << "RR";
                break;
            case ScheduleType::PRIORITY:
                std::cout << "PRIORITY";
                break;
            case ScheduleType::REALTIME:
                std::cout << "REALTIME";
                break;
        }
        std::cout << std::endl;

        // Pausar la ejecución para simular tiempo real
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

