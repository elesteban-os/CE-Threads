#include "flow/flow.h"
#include "schedulers/scheduler.h"
#include "processmanagement.h"
#include <thread>
#include "config/config_loader.h"

int main() {
    Configuration config;
    try
    {
        config = read_configuration("config/config.json");
        // imprimirConfiguracion(config);
    }
    catch(const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    // Tipo calendarizacion
    ScheduleType schedule_type;
    if (config.calendarizer == "FCFS") {
        schedule_type = ScheduleType::FCFS;
    } else if (config.calendarizer == "SJF") {
        schedule_type = ScheduleType::SJF;
    } else if (config.calendarizer == "RR") {
        schedule_type = ScheduleType::RR;
    } else if (config.calendarizer == "Prioridad") {
        schedule_type = ScheduleType::PRIORITY;
    } else if (config.calendarizer == "Real Time") {
        schedule_type = ScheduleType::REALTIME;
    } else {
        cerr << "Error: Unknown calendarizer type '" << config.calendarizer << "'" << endl;
        return 1;
    }

    // Tamanio de la cola
    int queue_size = config.parameters.queue_size;
    
    // Algoritmo de flujo
    FlowAlgorithm flow_algorithm;
    if (config.control_method == "Equidad") {
        flow_algorithm = FlowAlgorithm::EQUITY;
    } else if (config.control_method == "Letrero") {
        flow_algorithm = FlowAlgorithm::SIGN;
    } else if (config.control_method == "FIFO") {
        flow_algorithm = FlowAlgorithm::FIFO;
    } else {
        cerr << "Error: Unknown flow algorithm type '" << config.control_method << "'" << endl;
        return 1;
    }

    // Atributo compartido (W o Time)
    int flattr;
    if (flow_algorithm == FlowAlgorithm::EQUITY) {
        flattr = config.parameters.w_param;
    } else {
        flattr = config.parameters.sign_time;
    }

    int quantum = config.parameters.quantum;

    // Crear una instancia de ProcessManagement
    int street = config.street.size;
    ProcessManagement pm(schedule_type, queue_size, flow_algorithm, flattr, quantum, street);

    // Diferentes burst times:
    // y = mx
    // d = vt
    // t = d / v
    
    int base_speed = config.street.base_speed;
    int burst_time_normal = street / base_speed; // 10
    int burst_time_sport = street / (base_speed * 2); // 5
    int burst_time_emergency = street / (base_speed * 4); // 2.5

    // Agregar procesos al lado izquierdo
    // Prueba SJF
    // Agregar carros alternando el tipo
    int car_id = 1;
    int leftPeriod = 3;
    while (config.left.normal > 0 || config.left.deportive > 0 || config.left.emergency > 0) {
        if (config.left.normal > 0) {
            pm.newLeftProcess(Process(car_id++, burst_time_normal, 3, burst_time_normal * 20, burst_time_normal / 10, leftPeriod));
            config.left.normal--;
            leftPeriod += 3;
        }
        if (config.left.deportive > 0) {
            pm.newLeftProcess(Process(car_id++, burst_time_sport, 2, burst_time_sport * 18, burst_time_sport / 6, leftPeriod));
            config.left.deportive--;
            leftPeriod += 6;
        }
        if (config.left.emergency > 0) {
            pm.newLeftProcess(Process(car_id++, burst_time_emergency, 1, burst_time_emergency * 16, burst_time_emergency / 3, leftPeriod));
            config.left.emergency--;
            leftPeriod += 9;
        }
    }

    // Agregar procesos al lado derecho
    // Prueba SJF
    // Agregar carros alternando el tipo
    int rightPeriod = 1;
    while (config.right.normal > 0 || config.right.deportive > 0 || config.right.emergency > 0) {
        if (config.right.normal > 0) {
            pm.newRightProcess(Process(car_id++, burst_time_normal, 3, burst_time_normal * 10, burst_time_normal / 3, rightPeriod));
            config.right.normal--;
            rightPeriod += 3;
        }
        if (config.right.deportive > 0) {
            pm.newRightProcess(Process(car_id++, burst_time_sport, 2, burst_time_sport * 6, burst_time_sport / 3, rightPeriod));
            config.right.deportive--;
            rightPeriod += 6;
        }
        if (config.right.emergency > 0) {
            pm.newRightProcess(Process(car_id++, burst_time_emergency, 1, burst_time_emergency * 4, burst_time_emergency / 3, rightPeriod));
            config.right.emergency--;
            rightPeriod += 9;
        }
    }

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

        // Imprimir el algoritmo de flujo:
        std::cout << "Algoritmo de flujo: ";
        switch (data->flowAlgorithm) {
            case FlowAlgorithm::EQUITY:
            std::cout << "Equidad";
            break;
            case FlowAlgorithm::SIGN:
            std::cout << "Letrero";
            break;
            case FlowAlgorithm::FIFO:
            std::cout << "FIFO";
            break;
        }
        std::cout << std::endl;


        std::cout << "Tiempo dn movimiento: " << data->time << std::endl;
        std::cout << "Porcentaje de la calle que se debe mover " << data->street_percentage << "%" << std::endl;

        // 

        // Pausar la ejecución para simular tiempo real
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

