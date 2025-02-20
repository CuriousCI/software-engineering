#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>

class Network : public Timed,
                public Buffer<Payload>,
                public Notifier<Message> {

  public:
    Network(System *system)
        : Timed(system, 0, TimerMode::Once) {}

    void update(Payload payload) override {
        if (buffer.empty())
            timer.set_duration(2);
        Buffer<Payload>::update(payload);
    }

    void update(U) override {
        if (!buffer.empty()) {
            notify((Light)buffer.front());
            buffer.pop_front();
            if (!buffer.empty())
                timer.set_duration(2);
        }
    }
};
