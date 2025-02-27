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
    Stopwatch stopwatch;
    TrafficLight traffic_light;
    ControlCenter control_center(&system);

    system.addObserver(&monitor);
    system.addObserver(&stopwatch);
    network.addObserver(&traffic_light);
    traffic_light.addObserver(&monitor);
    control_center.addObserver(&network);
    control_center.addObserver(&monitor);

    std::ofstream file("logs");

    while (stopwatch.elapsed() <= HORIZON) {
        file << stopwatch.elapsed() << ' '
             << control_center.light() << ' '
             << traffic_light.light() << ' '
             << monitor.is_valid() << std::endl;

        system.next();
    }

    file.close();
    return 0;
}
