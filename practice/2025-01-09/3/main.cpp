#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/mocc.hpp"
#include "../../../mocc/system.hpp"
#include "customer.hpp"
#include "monitor.hpp"
#include "parameters.hpp"
#include <random>

int main() {
    std::random_device random_device;
    urng_t urng(random_device());

    real_t avg, var;

    {
        std::ifstream parameters("parameters.txt");

        std::string type;
        while (parameters >> type)
            if (type == "Avg")
                parameters >> avg;
            else if (type == "StdDev")
                parameters >> var;

        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Customer customer(urng, avg, var);
    Monitor monitor;

    system.addObserver(&stopwatch);
    stopwatch.addObserver(&customer);
    stopwatch.addObserver(&monitor);
    customer.addObserver(&monitor);

    while (stopwatch.elapsed() <= HORIZON)
        system.next();

    std::ofstream("results.txt")
        << "2025-01-09" << std::endl
        << "Avg " << monitor.interval_stat.mean() << std::endl
        << "StdDev " << monitor.interval_stat.stddev();

    return 0;
}
