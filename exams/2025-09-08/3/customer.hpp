#pragma once

#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include "server.hpp"

class Customer : public TimerBasedEntity {
    std::vector<Server *> &servers;

  public:
    size_t id;
    size_t count = 0;

    Customer(System &system, std::vector<Server *> &servers, size_t id)
        : TimerBasedEntity(system, A, TimerMode::Once, T), servers(servers),
          id(id) {}

    void update(TimerEnded) override {
        servers[random_server(urng)]->update(
            Demand{.id = id, .i = random_product(urng)}
        );

        this->timer.resetWithDuration(random_customer_wait_interval(urng));
    }
};
