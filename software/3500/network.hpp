#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>
#include <random>

class Network : public Timed,
                public Buffer<Payload>,
                public Notifier<Message>,
                public Notifier<Fault> {
    std::bernoulli_distribution random_fault;
    std::bernoulli_distribution random_repair;

  public:
    Network(System *system)
        : Timed(system, 0, TimerMode::Once),
          random_fault(0.01), random_repair(0.001) {}

    void update(Payload payload) override {
        if (buffer.empty())
            timer.set_duration(2);
        Buffer<Payload>::update(payload);
    }

    void update(U) override {
        if (!buffer.empty()) {
            if (random_fault(urng)) {
                if (random_repair(urng))
                    Notifier<Message>::notify(
                        (Light)buffer.front());
                else
                    Notifier<Fault>::notify(true);
            } else {
                Notifier<Message>::notify(
                    (Light)buffer.front());
            }

            buffer.pop_front();
            if (!buffer.empty())
                timer.set_duration(2);
        }
    }
};
