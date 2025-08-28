#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public TimerBasedEntity,
                 public Notifier<CustomerPurchaseRequest> {

    std::normal_distribution<> random_time_interval;

  public:
    Customer(System &system)
        : random_time_interval(AVG, VAR),
          TimerBasedEntity(system, AVG, TimerMode::Once) {}

    void update(TimerEnded) override {
        notify(CustomerPurchaseRequest{});
        timer.resetWithDuration(random_time_interval(urng));
    }
};
