#pragma once

#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>

class ControlCenter : public TimerBasedEntity,
                      public Notifier<NetworkPayloadLight> {
    std::uniform_int_distribution<> random_interval;
    Light l = Light::RED;

  public:
    ControlCenter(System &system)
        : random_interval(60, 120),
          TimerBasedEntity(system, 90, TimerMode::Once) {}

    void update(TimerEnded) override {
        l = (l == RED ? GREEN : (l == GREEN ? YELLOW : RED));
        notify(l);
        timer.resetWithDuration(random_interval(urng));
    }

    Light light() { return l; }
};
