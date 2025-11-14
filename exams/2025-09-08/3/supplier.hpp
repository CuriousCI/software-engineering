#pragma once

#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include "server.hpp"

class Supplier : public TimerBasedEntity {
    std::vector<Server *> &servers;

  public:
    size_t id;

    Supplier(System &system, std::vector<Server *> &servers, size_t id)
        : TimerBasedEntity(system, V, TimerMode::Once, T), servers(servers),
          id(id) {}

    void update(TimerEnded) override {
        servers[random_server(urng)]->update(
            Supply{.id = id, .i = random_product(urng)}
        );

        this->timer.resetWithDuration(random_supplier_wait_interval(urng));
    }
};
