#pragma once

#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"

class TrafficLight : public Timed {
    std::uniform_int_distribution<> random_interval;
    Light l = Light::RED;

  public:
    TrafficLight(System *system)
        : random_interval(60, 120),
          Timed(system, 90, TimerMode::Once) {}

    void update(U) override {
        l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
        timer.set_duration(random_interval(urng));
    }

    Light light() { return l; }
};
