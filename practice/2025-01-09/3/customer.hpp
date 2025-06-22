#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public TimerBasedEntity,
                 public Recorder<StopwatchElapsedTime>,
                 public Notifier<PurchaseRequest> {

    std::normal_distribution<> random_interval;

  public:
    Customer(System &system)
        : random_interval(AVG, VAR),
          TimerBasedEntity(system, AVG, TimerMode::Once) {}

    void update(TimerEnded) override {
        notify(PurchaseRequest{record});
        timer.resetWithDuration(random_interval(urng));
    }
};
