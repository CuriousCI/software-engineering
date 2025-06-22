#include <cstdlib>
#include <fstream>
#include <iostream>

#include "../../../mocc/system.hpp"
#include "customer.hpp"
#include "dispatcher.hpp"
#include "monitor.hpp"
#include "parameters.hpp"

int main() {

    {
        std::ifstream parameters("parameters.txt");
        std::string type;
        while (parameters >> type)
            if (type == "N")
                parameters >> N;
            else if (type == "Avg")
                parameters >> AVG;
            else if (type == "StdDev")
                parameters >> VAR;
        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Dispatcher dispatcher(N);
    Monitor monitor;
    std::vector<Customer *> customers;

    for (size_t id = 1; id <= N; id++) {
        Customer *customer = new Customer(system, id);

        stopwatch.addObserver(customer);
        customer->addObserver(&dispatcher);

        customers.push_back(customer);
    }

    system.addObserver(&stopwatch);
    system.addObserver(&dispatcher);
    dispatcher.Notifier<PurchaseRequest>::addObserver(&monitor);

    while (stopwatch.elapsedTime() <= HORIZON)
        system.next();

    {
        std::ofstream output("results.txt");
        output << "2025-01-09" << std::endl;
        for (auto customer : customers)
            output << customer->id << " "
                   << dispatcher.requests_count[customer->id - 1] << std::endl;
        output << "M2 " << (monitor.preserves_order ? 0 : 1);
        output.close();
    }

    return 0;
}
