#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "director.hpp"
#include "employee.hpp"
#include "parameters.hpp"

#include <fstream>
#include <iostream>

int main() {
    {
        std::ifstream parameters("parameters.txt");
        char c;

        // clang-format off
        while (parameters >> c)
            switch (c) {
                case 'A': parameters >> A; break;
                case 'B': parameters >> B; break;
                case 'C': parameters >> C; break;
                case 'D': parameters >> D; break;
                case 'F': parameters >> F; break;
                case 'G': parameters >> G; break;
                case 'N': parameters >> N; break;
                case 'W': parameters >> W; break;
            }
        // clang-format on

        parameters.close();
    }

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
    while (stopwatch.elapsedTime() < HORIZON) {
        bool terminate = stopwatch.elapsedTime() > 1000;
        for (auto employee : employees)
            if (employee->comp_time_data.stddev() >
                0.01 * employee->comp_time_data.mean()) {
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
            << "AvgTime " << director.time_data.mean() << std::endl
            << "AvgCost " << director.cost_data.mean() << std::endl;

        for (auto employee : employees)
            std::ofstream("output.txt", std::ios_base::app)
                << employee->id << ' ' 
                << employee->comp_time_data.mean() << ' ' 
                << employee->comp_time_data.mean() * employee->cost << ' ' 
                << employee->comp_time_data.stddev() << ' '
                << employee->comp_time_data.stddev() * employee->cost
                << std::endl;
        // clang-format on
    }

    return 0;
}
