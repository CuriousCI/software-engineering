#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>
#include <random>

class Network : public TimerBasedEntity,
                public Buffer<NetworkPayloadLight>,
                public Notifier<LightUpdateMessage> {
    std::bernoulli_distribution random_fault;

  public:
    Network(System &system)
        : TimerBasedEntity(system, 0, TimerMode::Once), random_fault(0.05) {}

    void update(NetworkPayloadLight payload) override {
        if (buffer.empty())
            timer.resetWithDuration(2);
        Buffer<NetworkPayloadLight>::update(payload);
    }

    void update(TimerEnded) override {
        if (!buffer.empty()) {
            if (!random_fault(urng))
                notify((Light)buffer.front());
            buffer.pop_front();
            if (!buffer.empty())
                timer.resetWithDuration(2);
        }
    }
};
