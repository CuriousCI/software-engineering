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

        std::string line_type;
        while (parameters >> line_type)
            if (line_type == "N")
                parameters >> N;
            else if (line_type == "Avg")
                parameters >> AVG;
            else if (line_type == "StdDev")
                parameters >> VAR;

        parameters.close();
    }

    System system;
    Stopwatch stopwatch(T);
    Dispatcher dispatcher(N);
    Monitor monitor(N);
    std::vector<Customer *> customers;

    for (size_t id = 1; id <= N; id++) {
        Customer *customer = new Customer(system, id);

        stopwatch.addObserver(customer);
        customer->addObserver(&dispatcher);

        customers.push_back(customer);
    }

    system.addObserver(&stopwatch);
    system.addObserver(&dispatcher);
    dispatcher.Notifier<CustomerId, RequestsCount>::addObserver(&monitor);

    while (stopwatch.elapsedTime() <= HORIZON)
        system.next();

    {
        std::ofstream output("results.txt");
        output << "2025-01-09" << std::endl;
        for (auto customer : customers)
            output << customer->id << " "
                   << dispatcher.requests_count[customer->id - 1] << std::endl;
        output << "M1 " << monitor.total_requests_received;
        output.close();
    }

    return 0;
}
