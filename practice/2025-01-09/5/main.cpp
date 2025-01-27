#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/mocc.hpp"
#include "../../../mocc/system.hpp"
#include "customer.hpp"
#include "dispatcher.hpp"
#include "monitor.hpp"
#include "parameters.hpp"
#include <random>

int main() {
    std::random_device random_device;
    urng_t urng(random_device());

    size_t N;
    real_t avg, var;

    {
        std::ifstream parameters("parameters.txt");
        std::string type;
        while (parameters >> type)
            if (type == "N")
                parameters >> N;
            else if (type == "Avg")
                parameters >> avg;
            else if (type == "StdDev")
                parameters >> var;
        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Dispatcher dispatcher(N);
    Monitor monitor;
    std::vector<Customer *> customers;

    for (size_t i = 1; i <= N; i++) {
        Customer *c = new Customer(urng, avg, var, i);

        stopwatch.addObserver(c);
        c->addObserver(&dispatcher);

        customers.push_back(c);
    }

    system.addObserver(&stopwatch);
    system.addObserver(&dispatcher);
    dispatcher.Notifier<Request>::addObserver(&monitor);

    while (stopwatch.elapsed() <= HORIZON)
        system.next();

    {
        std::ofstream output("results.txt");
        output << "2025-01-09" << std::endl;
        for (auto c : customers)
            output << c->i << " "
                   << dispatcher.requests_count[c->i - 1]
                   << std::endl;
        output << "M2 " << (monitor.preserves_order ? 0 : 1);
        output.close();
    }

    return 0;
}
