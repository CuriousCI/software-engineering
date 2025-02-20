#include <cstdlib>
#include <fstream>

#include "../../mocc/system.hpp"
#include "control_center.hpp"
#include "monitor.hpp"
#include "network.hpp"
#include "parameters.hpp"
#include "traffic_light.hpp"

int main() {
    System system;
    Monitor monitor;
    Network network(&system);
    TrafficLight traffic_light;
    ControlCenter control_center(&system);

    system.addObserver(&monitor);
    network.addObserver(&traffic_light);
    traffic_light.addObserver(&monitor);
    control_center.addObserver(&network);
    control_center.addObserver(&monitor);

    std::ofstream file("logs");

    for (size_t time = 0; time <= HORIZON; time++) {
        file << time << ' ' << control_center.light() << ' '
             << traffic_light.light() << ' '
             << monitor.is_valid() << std::endl;

        system.next();
    }

    file.close();
    return 0;
}
