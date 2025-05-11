#include "config_loader.h"

/*
 * Metodo para la validacion de los valores ingresados en el archivo de configuracion
 */
void validate_configuration(const Configuration &config) {

    // Validacion de los metodos de control
    if (config.control_method != "Equidad" && config.control_method != "Letrero" && config.control_method != "FIFO") {
        throw runtime_error("Method of control not recognized");
    }
    // Validacion de los metodos de calendarizacion
    if (config.calendarizer != "RR" && config.calendarizer != "Prioridad" && config.calendarizer != "SJF" && config.calendarizer != "FCFS" && config.calendarizer != "Real Time") {
        throw runtime_error("Method of calendarization not recognized");
    }
    
    // Validacion de los parametro queue_size
    if (config.parameters.queue_size < 0) {
        throw runtime_error("Queue size must be greater than 0");
    }
    
    // Validacion de los parametro w_param
    if (config.parameters.w_param < 0) {
        throw runtime_error("W param must be greater than 0");
    }

    // Validacion de los parametro sign_time
    if (config.parameters.sign_time < 0) {
        throw runtime_error("Sign time must be greater than 0");
    }

    // Validacion de los parametros de la calle
    if (config.street.size <= 0 || config.street.size <= 0) {
        throw runtime_error("Street parameters must be greater than 0");
    }
  
    int total_left = config.left.normal + config.left.deportive + config.left.emergency;
    int total_right = config.right.normal + config.right.deportive + config.right.emergency;

    // Validacion de cantidad de carros
    if (total_left + total_right > config.parameters.queue_size) {
        throw runtime_error("Total cars exceed queue size");
    }
    if (total_left > config.parameters.queue_size) {
        throw runtime_error("Left extreme has more cars than queue size");
    } 
    if (total_right > config.parameters.queue_size) {
        throw runtime_error("Right extreme has more cars than queue size");
    }
}

/*
 * Metodo para la lectura del archivo de configuracion
 */
Configuration read_configuration(const string& filename) {
    
    // Abre el archivo de configuracion
    ifstream config_file(filename);
    if (!config_file.is_open()) {
        throw runtime_error("Could not open configuration file");
    }

    // Crea un objeto json
    json data;
    
    // Trata de leer el archivo de configuracion
    try {
        data = json::parse(config_file);
    } catch (const json::parse_error& e) {
        throw runtime_error("Error parsing JSON: " + string(e.what()));
    }

    // Crea un objeto de configuracion
    Configuration config;

    // Trata de parsear y obtener los datos del archivo de configuracion
    try {

        // Parsea el metodo de control y el metodo de calendarizacion
        config.control_method = data.at("metodo_control").get<string>();
        config.calendarizer = data.at("calendarizador").get<string>();

        // Parsea los parametros
        config.parameters.queue_size = data.at("parametros").at("largo_cola").get<int>();
        config.parameters.w_param = data.at("parametros").at("W").get<int>();
        config.parameters.sign_time = data.at("parametros").at("tiempo_letreto").get<int>();

        // Parsea los parametros de la calle
        config.street.size = data.at("calle").at("largo").get<int>();
        config.street.base_speed = data.at("calle").at("velocidad_base").get<int>();

        // Parsea cantidad de carros en la izquierda
        config.left.normal = data.at("carros").at("izquierda").at("normal").get<int>();
        config.left.deportive = data.at("carros").at("izquierda").at("deportivo").get<int>();
        config.left.emergency = data.at("carros").at("izquierda").at("emergencia").get<int>();
        
        // Parsea cantidad de carros en la derecha
        config.right.normal = data.at("carros").at("derecha").at("normal").get<int>();
        config.right.deportive = data.at("carros").at("derecha").at("deportivo").get<int>();
        config.right.emergency = data.at("carros").at("derecha").at("emergencia").get<int>();

    } catch (const json::exception& e) {
        throw runtime_error("Error reading JSON: " + string(e.what()));
    }

    // Valida la configuracion
    validate_configuration(config);

    // Retorna el objeto de configuracion
    return config;  //  Deberia de retornar los valores de la configuracion en un objeto como esta hecho o deberia de retornar una struct con todo dentro? O como deberia de retornar esos valores?

   
}

