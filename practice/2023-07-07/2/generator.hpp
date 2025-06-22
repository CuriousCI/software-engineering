#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Generator : public Observer<StopwatchElapsedTime>, public Notifier<Job> {
    size_t job = 1;
    std::bernoulli_distribution send;
    urng_t &urng;

  public:
    Generator(urng_t &urng) : urng(urng), send(1. - 1. / L) {}

    void update(StopwatchElapsedTime time) override {
        if (send(urng)) {
            notify(job);
            job++;
        }
    }
};
