#pragma once

#include "../../../mocc/buffer.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Dispatcher : public Observer<>,
                   public Buffer<Job>,
                   public Notifier<Task> {
    Timer timer;
    urng_t &urng;
    std::uniform_int_distribution<> random_worker;

  public:
    Dispatcher(urng_t &urng, System *system)
        : Buffer(B), urng(urng), timer(D, TimerMode::Repeating),
          random_worker(W) {
        system->addObserver(&timer);
        // timer.addObserver(this);
    }

    void update() override {
        if (!buffer.empty()) {
            notify({buffer.front(), 1});
            buffer.pop_front();
        }
    }

    void notify(Task t) override { observers[random_worker(urng)]->update(t); }
};
