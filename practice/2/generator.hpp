#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include "../../mocc/notifier.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Generator : public Observer<T>, public Notifier<Job> {
    size_t job = 1;
    std::bernoulli_distribution send;
    urng_t &urng;

  public:
    Generator(urng_t &urng) : urng(urng), send(1. - 1. / L) {}

    void update(T t) override {
        if (send(urng)) {
            notify(job);
            job++;
        }
    }
};

#endif
