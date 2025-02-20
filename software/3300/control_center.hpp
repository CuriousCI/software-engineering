#pragma once

#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>

class ControlCenter : public Timed, public Notifier<Payload> {
    std::uniform_int_distribution<> random_interval;
    Light l = Light::RED;

  public:
    ControlCenter(System *system)
        : random_interval(60, 120),
          Timed(system, 90, TimerMode::Once) {}

    void update(U) override {
        l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
        notify(l);
        timer.set_duration(random_interval(urng));
    }

    Light light() { return l; }
};
