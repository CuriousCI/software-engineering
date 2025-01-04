#ifndef DISTURBANCE_HPP_
#define DISTURBANCE_HPP_

#include <random>

#include "../case/case.hpp"
#include "../case/time.hpp"
#include "data.hpp"

class Disturbance : public Subscriber<Time>, public Publisher<Dist> {
    std::default_random_engine &random_engine;
    std::uniform_real_distribution<> random_disturbance =
        std::uniform_real_distribution<>(0, 2);

    real_t next_sampling = 0;

  public:
    Disturbance(std::default_random_engine &);
    void update(Time) override;
};

#endif
