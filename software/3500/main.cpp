#include <cstdlib>
#include <fstream>

#include "../../mocc/notifier.hpp"
#include "../../mocc/system.hpp"
#include "control_center.hpp"
#include "monitor.hpp"
#include "network.hpp"
#include "parameters.hpp"
#include "traffic_light.hpp"

int main() {
    System system;
    Monitor monitor;
    Stopwatch stopwatch;
    Network network(system);
    TrafficLight traffic_light;
    ControlCenter control_center(system);

    system.addObserver(&monitor);
    system.addObserver(&stopwatch);
    network.Notifier<LightUpdateMessage>::addObserver(&traffic_light);
    network.Notifier<Fault>::addObserver(&traffic_light);
    traffic_light.addObserver(&monitor);
    control_center.addObserver(&network);
    control_center.addObserver(&monitor);

    std::ofstream file("logs");

    while (stopwatch.elapsedTime() <= HORIZON) {
        file << stopwatch.elapsedTime() << ' ' << control_center.light() << ' '
             << traffic_light.light() << ' ' << monitor.isValid() << std::endl;

        system.next();
    }

    file.close();
    return 0;
}
