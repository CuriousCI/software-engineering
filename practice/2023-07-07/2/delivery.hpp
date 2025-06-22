#pragma once

#include "../../../mocc/buffer.hpp"
#include "../../../mocc/observer.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Delivery : public SystemObserver, public Buffer<TaskDone> {
    Timer timer;

  public:
    Delivery(System *system) : Buffer(B), timer(D, TimerMode::Repeating) {
        system->addObserver(&timer);
        // timer.addObserver(this);
    }

    void update() override {
        if (!buffer.empty())
            buffer.pop_front();
    }
};
