#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/math.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "customer.hpp"
#include "parameters.hpp"
#include "server.hpp"
#include "supplier.hpp"
#include <random>

int main() {
    {
        std::ifstream parameters("parameters.txt");
        char line_type;
        // clang-format off
        parameters >> 
            line_type >> T >> 
            line_type >> H >> 
            line_type >> M >>
            line_type >> C >> 
            line_type >> A >> 
            line_type >> B >> 
            line_type >> F >> 
            line_type >> V >> 
            line_type >> Q >> 
            line_type >> P >>
            line_type >> S >>
            line_type >> K;
        // clang-format on

        parameters.close();

        random_server = std::uniform_int_distribution<>(0, S - 1);
        random_product = std::uniform_int_distribution<>(0, P - 1);
        random_initial_product_amout = std::uniform_int_distribution<>(0, K);
        random_customer_wait_interval = std::uniform_real_distribution<>(A, B);
        random_supplier_wait_interval = std::uniform_real_distribution<>(V, Q);
    }

    OnlineDataAnalysis missed_sells_data;
    for (size_t simulation = 0; simulation < M; simulation++) {
        System system;
        Stopwatch stopwatch(T);
        std::vector<Customer *> customers;
        std::vector<Server *> servers;
        std::vector<Supplier *> suppliers;

        system.addObserver(&stopwatch);
        for (size_t _ = 0; _ < S; _++) {
            Server *server = new Server();
            servers.push_back(server);
            system.addObserver(server);
        }

        for (size_t id = 0; id < C; id++) {
            Customer *customer = new Customer(system, servers, id);
            customers.push_back(customer);
        }

        for (size_t id = 0; id < F; id++) {
            Supplier *supplier = new Supplier(system, servers, id);
            suppliers.push_back(supplier);
        }

        while (stopwatch.elapsedTime() <= H) {
            system.next();
        }

        size_t total_missed_sells = 0;
        for (auto server : servers) {
            total_missed_sells += server->missed_sells;
        }

        missed_sells_data.insertDataPoint(total_missed_sells);
    }

    std::ofstream("results.txt")
        << "2025-01-09\nR " << missed_sells_data.mean() / (real_t)H;
    return 0;
}
