#include <fstream>

#include "parameters.hpp"
#include "traffic_light.hpp"

int main() {
    System system;
    Stopwatch stopwatch;
    TrafficLight traffic_light(&system);

    system.addObserver(&stopwatch);

    std::ofstream file("logs");

    while (stopwatch.elapsed() <= HORIZON) {
        file << stopwatch.elapsed() << ' '
             << traffic_light.light() << std::endl;
        system.next();
    }

    file.close();
    return 0;
}
