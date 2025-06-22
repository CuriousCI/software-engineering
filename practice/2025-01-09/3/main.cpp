#include <fstream>
#include <iostream>

#include "../../../mocc/system.hpp"
#include "customer.hpp"
#include "monitor.hpp"
#include "parameters.hpp"

int main() {
    {
        std::ifstream parameters("parameters.txt");

        std::string type;
        while (parameters >> type)
            if (type == "Avg")
                parameters >> AVG;
            else if (type == "StdDev")
                parameters >> VAR;

        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Customer customer(system);
    Monitor monitor;

    system.addObserver(&stopwatch);
    stopwatch.addObserver(&customer);
    stopwatch.addObserver(&monitor);
    customer.addObserver(&monitor);

    while (stopwatch.elapsedTime() <= HORIZON)
        system.next();

    std::ofstream("results.txt")
        << "2025-01-09" << std::endl
        << "Avg " << monitor.intervals_data.mean() << std::endl
        << "StdDev " << monitor.intervals_data.stddev();

    return 0;
}
