#include <fstream>

#include "parameters.hpp"
#include "traffic_light.hpp"

int main() {
    System system;
    TrafficLight traffic_light(&system);

    std::ofstream file("logs");

    for (size_t time = 0; time <= HORIZON; time++) {
        file << time << ' ' << traffic_light.light()
             << std::endl;
        system.next();
    }

    file.close();
    return 0;
}
