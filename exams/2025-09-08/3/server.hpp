#pragma once

#include "../../../mocc/observer.hpp"
#include "../../../mocc/system.hpp"
#include "parameters.hpp"
#include <deque>

class Server : public SystemObserver,
               public Observer<Demand>,
               public Observer<Supply> {

    std::vector<size_t> database;
    std::vector<std::deque<ProductNumber>> customers_fifo;
    std::vector<std::deque<ProductNumber>> suppliers_fifo;

  public:
    size_t missed_sells = 0;

    Server() : database(P, 0), customers_fifo(), suppliers_fifo() {
        for (size_t customer = 0; customer <= C; customer++) {
            customers_fifo.push_back(std::deque<ProductNumber>());
        }

        for (size_t supplier = 0; supplier <= C; supplier++) {
            suppliers_fifo.push_back(std::deque<ProductNumber>());
        }
    }

    void update() override {
        for (size_t supplier = 0; supplier < F; supplier++) {
            if (!suppliers_fifo[supplier].empty()) {
                auto i = suppliers_fifo[supplier].front();
                suppliers_fifo[supplier].pop_front();
                database[i]++;
            }
        }

        // while (!Buffer<ProductNumberSupply>::buffer.empty()) {
        //     auto i = this->Buffer<ProductNumberSupply>::buffer.front();
        //     Buffer<ProductNumberSupply>::buffer.pop_front();
        //     database[i]++;
        // }

        for (size_t customer = 0; customer < C; customer++) {
            if (!customers_fifo[customer].empty()) {

                auto i = customers_fifo[customer].front();
                customers_fifo[customer].pop_front();

                if (database[i] > 0) {
                    database[i]--;
                } else {
                    missed_sells++;
                }
            }
        }
    }

    void update(Demand demand) override {
        customers_fifo[demand.id].push_back(demand.i);
    }

    void update(Supply supply) override {
        suppliers_fifo[supply.id].push_back(supply.i);
    }
};
