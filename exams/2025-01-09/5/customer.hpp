#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public TimerBasedEntity,
                 public Recorder<StopwatchElapsedTime>,
                 public Notifier<CustomerId, CustomerPurchaseRequest> {

    std::normal_distribution<> random_interval;

  public:
    const size_t id;

    Customer(System &system, size_t id)
        : random_interval(AVG, VAR),
          TimerBasedEntity(system, AVG, TimerMode::Once), id(id) {}

    void update(TimerEnded) override {
        notify(id, CustomerPurchaseRequest{.time = record});
        timer.resetWithDuration(random_interval(urng));
    }
};
