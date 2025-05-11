#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Parameters {
    int queue_size;
    int w_param;
    int sign_time;
};

struct Street {
    int size;
    int base_speed;
};

struct Car_type {
    int normal;
    int deportive;
    int emergency;
};

struct Configuration {
    string control_method;
    string calendarizer;
    Parameters parameters;
    Street street;
    Car_type left;
    Car_type right;
};

void validate_configuration(const Configuration& config);

Configuration read_configuration(const string& filename);