#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "director.hpp"
#include "employee.hpp"
#include "parameters.hpp"
#include <fstream>
#include <iostream>

/* 07/07/2023 - ex2 */

int main() {
    {
        std::ifstream params("parameters.txt");
        char c;

        // clang-format off
        while (params >> c)
            switch (c) {
            case 'A': params >> A; break;
            case 'B': params >> B; break;
            case 'C': params >> C; break;
            case 'D': params >> D; break;
            case 'F': params >> F; break;
            case 'G': params >> G; break;
            case 'N': params >> N; break;
            case 'W': params >> W; break;
            }
        // clang-format on

        params.close();
    }

    /* Setup */
    std::random_device random_device;
    urng_t urng(random_device());

    System system;
    Director director;
    Stopwatch stopwatch;
    std::vector<Employee *> employees;

    {
        system.addObserver(&stopwatch);

        for (size_t k = 1; k <= W; k++) {
            auto e = new Employee(urng, k);

            e->addObserver(&director);
            director.addObserver(e);
            stopwatch.addObserver(e);
            employees.push_back(e);
        }
    }

    /* Simulation */
    while (stopwatch.elapsed() < HORIZON) {
        bool terminate = stopwatch.elapsed() > 1000;
        for (auto e : employees)
            if (e->comp_time_stat.stddev() >
                0.01 * e->comp_time_stat.mean()) {
                terminate = false;
                break;
            }

        if (terminate)
            break;

        system.next();
    }

    {
        // clang-format off
        std::ofstream ("output.txt") 
            << "AvgTime " << director.time_stat.mean() << std::endl
            << "AvgCost " << director.cost_stat.mean() << std::endl;

        for (auto e : employees)
            std::ofstream("output.txt", std::ios_base::app)
                << e->id << ' ' 
                << e->comp_time_stat.mean() << ' ' 
                << e->comp_time_stat.mean() * e->cost << ' ' 
                << e->comp_time_stat.stddev() << ' '
                << e->comp_time_stat.stddev() * e->cost
                << std::endl;
        // clang-format on
    }

    return 0;
}
