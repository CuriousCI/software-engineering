#pragma once

#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"

class TrafficLight : public TimerBasedEntity {
    std::uniform_int_distribution<> random_interval;
    Light l = Light::RED;

  public:
    TrafficLight(System &system)
        : random_interval(60, 120),
          TimerBasedEntity(system, 90, TimerMode::Once) {}

    void update(TimerEnded) override {
        l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
        timer.resetWithDuration(random_interval(urng));
    }

    Light light() { return l; }
};
