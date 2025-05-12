#include "config_loader.h"

// Función para imprimir la configuración
void imprimirConfiguracion(const Configuration& config) {
    
    // Encabezado
    cout << "\n=== Configuración del Simulador ===\n\n";
    
    // Método y calendarizador
    cout << left << setw(20) << "Método control:" << config.control_method << "\n";
    cout << setw(20) << "Calendarizador:" << config.calendarizer << "\n\n";
    
    // Parámetros
    cout << "--- Parámetros ---\n";
    cout << setw(20) << "Largo cola:" << config.parameters.queue_size << "\n";
    cout << setw(20) << "W (equidad):" << config.parameters.w_param << "\n";
    cout << setw(20) << "Cambio letrero:" << config.parameters.sign_time << " seg\n\n";
    
    // Calle
    cout << "--- Calle ---\n";
    cout << setw(20) << "Largo:" << config.street.size << " m\n";
    cout << setw(20) << "Velocidad base:" << config.street.base_speed << " km/h\n\n";
    
    // Carros - Izquierda
    cout << "--- Carros Izquierda ---\n";
    cout << setw(20) << "Normales:" << config.left.normal << "\n";
    cout << setw(20) << "Deportivos:" << config.left.deportive << "\n";
    cout << setw(20) << "Emergencia:" << config.left.emergency << "\n\n";
    
    // Carros - Derecha
    cout << "--- Carros Derecha ---\n";
    cout << setw(20) << "Normales:" << config.right.normal << "\n";
    cout << setw(20) << "Deportivos:" << config.right.deportive << "\n";
    cout << setw(20) << "Emergencia:" << config.right.emergency << "\n";
    cout << "\n===============================\n";
}


int main() {
    try
    {
        Configuration config = read_configuration("config/config.json");
        imprimirConfiguracion(config);
    }
    catch(const exception& e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
    
}

// sudo apt-get update
// sudo apt-get install nlohmann-json3-dev
// g++ -std=c++11 config_test.cpp config_loader.cpp -o config_test