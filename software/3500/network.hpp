#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>
#include <random>

class Network : public TimerBasedEntity,
                public Buffer<NetworkPayloadLight>,
                public Notifier<LightUpdateMessage>,
                public Notifier<Fault> {
    std::bernoulli_distribution random_fault;
    std::bernoulli_distribution random_repair;

  public:
    Network(System &system)
        : TimerBasedEntity(system, 0, TimerMode::Once), random_fault(0.01),
          random_repair(0.001) {}

    void update(NetworkPayloadLight payload) override {
        if (buffer.empty())
            timer.resetWithDuration(2);
        Buffer<NetworkPayloadLight>::update(payload);
    }

    void update(TimerEnded) override {
        if (!buffer.empty()) {
            if (random_fault(urng)) {
                if (random_repair(urng))
                    Notifier<LightUpdateMessage>::notify((Light)buffer.front());
                else
                    Notifier<Fault>::notify(true);
            } else {
                Notifier<LightUpdateMessage>::notify((Light)buffer.front());
            }

            buffer.pop_front();
            if (!buffer.empty())
                timer.resetWithDuration(2);
        }
    }
};
