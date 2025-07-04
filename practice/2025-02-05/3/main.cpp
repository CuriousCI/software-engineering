#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "../../../mocc/math.hpp"
#include "../../../mocc/server.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "customer.hpp"
#include "database.hpp"
#include "parameters.hpp"
#include "server.hpp"
#include "supplier.hpp"

// each server has a 1000 FIFO size

int main() {

    {

        std::ifstream parameters("parameters.txt");
        parameters >> H;
        parameters >> n;
        parameters >> k;

        parameters >> alpha;
        postgresql_update = std::bernoulli_distribution(alpha);

        std::vector<real_t> items_purchase_probabilities;
        for (size_t _ = 0; _ <= k; _++) {
            real_t probability;
            parameters >> probability;
            items_purchase_probabilities.push_back(probability);
        }
        items_purchase_distribution =
            std::discrete_distribution(items_purchase_probabilities.begin(),
                                       items_purchase_probabilities.end());

        std::vector<real_t> items_supply_probabilities;
        for (size_t _ = 0; _ <= k; _++) {
            real_t probability;
            parameters >> probability;
            items_supply_probabilities.push_back(probability);
        }

        items_supply_distribution =
            std::discrete_distribution(items_supply_probabilities.begin(),
                                       items_supply_probabilities.end());

        parameters.close();
    }

    Data oversellings_data;
    for (size_t _ = 0; _ < 1000; _++) {
        System system;
        Stopwatch stopwatch(T);

        PostgreSQL database;
        Supplier supplier;

        system.addObserver(&stopwatch);
        system.addObserver(&supplier);
        supplier.addObserver(&database);

        auto customers = std::vector<Customer *>();
        for (size_t _ = 0; _ < n; _++) {
            auto c = new Customer();
            system.addObserver(c);

            customers.push_back(c);
        }

        auto servers = std::vector<Server *>();
        for (size_t i = 0; i < n; i++) {
            auto s = new Server(system);

            customers[i]->addObserver(s);
            s->Client<SellsUpdateRequest, CacheUpdateResponse>::addObserver(
                &database);

            servers.push_back(s);
        }

        try {
            while (stopwatch.elapsedTime() <= H) {
                system.next();
            }
        } catch (buffer_full) {
        }

        oversellings_data.insertDataPoint(database.total_oversellings);
    }

    {
        std::ofstream output("results.txt");
        output << "2025-02-05" << std::endl;
        output << "S " << (size_t)oversellings_data.mean() << std::endl;
        output << "R " << oversellings_data.mean() / (n * H) << std::endl;
        output.close();
    }

    return 0;
}

// for (auto customer : customers)
//     output << customer->id << " "
//            << dispatcher.requests_count[customer->id - 1] <<
//            std::endl;
// output << "M2 " << (monitor.preserves_order ? 0 : 1);
//

// #include <cstdlib>
// #include <fstream>
// #include <iostream>
//
// #include "../../../mocc/system.hpp"
// #include "customer.hpp"
// #include "dispatcher.hpp"
// #include "monitor.hpp"
// #include "parameters.hpp"
//
// int main() {
//
//     {
//         std::ifstream parameters("parameters.txt");
//         std::string type;
//         while (parameters >> type)
//             if (type == "N")
//                 parameters >> N;
//             else if (type == "Avg")
//                 parameters >> AVG;
//             else if (type == "StdDev")
//                 parameters >> VAR;
//         parameters.close();
//     }
//
//     System system;
//     Stopwatch stopwatch(T);
//     Dispatcher dispatcher(N);
//     Monitor monitor;
//     std::vector<Customer *> customers;
//
//     for (size_t id = 1; id <= N; id++) {
//         Customer *customer = new Customer(system, id);
//
//         stopwatch.addObserver(customer);
//         customer->addObserver(&dispatcher);
//
//         customers.push_back(customer);
//     }
//
//     system.addObserver(&stopwatch);
//     system.addObserver(&dispatcher);
//     dispatcher.Notifier<PurchaseRequest>::addObserver(&monitor);
//
//     while (stopwatch.elapsedTime() <= HORIZON)
//         system.next();
//
//     {
//         std::ofstream output("results.txt");
//         output << "2025-01-09" << std::endl;
//         for (auto customer : customers)
//             output << customer->id << " "
//                    << dispatcher.requests_count[customer->id - 1] <<
//                    std::endl;
//         output << "M2 " << (monitor.preserves_order ? 0 : 1);
//         output.close();
//     }
//
//     return 0;
// }
