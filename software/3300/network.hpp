#pragma once

#include "../../mocc/buffer.hpp"
#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <cstdlib>
#include <random>

class Network : public Timed,
                public Buffer<Payload>,
                public Notifier<Message> {
    std::bernoulli_distribution random_fault;

  public:
    Network(System *system)
        : Timed(system, 0, TimerMode::Once),
          random_fault(0.01) {}

    void update(Payload payload) override {
        if (buffer.empty())
            timer.set_duration(2);
        Buffer<Payload>::update(payload);
    }

    void update(U) override {
        if (!buffer.empty()) {
            if (!random_fault(urng))
                notify((Light)buffer.front());
            buffer.pop_front();
            if (!buffer.empty())
                timer.set_duration(2);
        }
    }
};
