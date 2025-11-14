#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>

class Network : public TimerBasedEntity,
                public Buffer<NetworkPayloadLight>,
                public Notifier<LightUpdateMessage> {

  public:
    Network(System &system) : TimerBasedEntity(system, 0, TimerMode::Once) {}

    void update(NetworkPayloadLight payload) override {
        if (buffer.empty())
            timer.resetWithDuration(2);
        Buffer<NetworkPayloadLight>::update(payload);
    }

    void update(TimerEnded) override {
        if (!buffer.empty()) {
            notify((Light)buffer.front());
            buffer.pop_front();
            if (!buffer.empty())
                timer.resetWithDuration(2);
        }
    }
};
