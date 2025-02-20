#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public Timed,
                 public Recorder<class T>,
                 public Notifier<Request> {

    real_t next_request = 0;
    std::normal_distribution<> random_interval;

  public:
    Customer(System *system, real_t avg, real_t var)
        : random_interval(avg, var),
          Timed(system, avg, TimerMode::Once) {}

    void update(U) override {
        notify(Request{record});
        timer.set_duration(random_interval(urng));
    }
};
