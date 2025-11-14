#include <fstream>
#include <iostream>

#include "../../../mocc/system.hpp"
#include "customer.hpp"
#include "monitor.hpp"
#include "parameters.hpp"

int main() {
    {
        std::ifstream parameters("parameters.txt");

        std::string line_type;
        while (parameters >> line_type)
            if (line_type == "Avg")
                parameters >> AVG;
            else if (line_type == "StdDev")
                parameters >> VAR;

        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Customer customer(system);
    Monitor monitor;

    system.addObserver(&stopwatch);
    stopwatch.addObserver(&monitor);
    customer.addObserver(&monitor);

    while (stopwatch.elapsedTime() <= HORIZON)
        system.next();

    std::ofstream("results.txt")
        << "2025-01-09" << std::endl
        << "Avg " << monitor.requests_interval_analysis.mean() << std::endl
        << "StdDev " << monitor.requests_interval_analysis.stddev();

    return 0;
}
