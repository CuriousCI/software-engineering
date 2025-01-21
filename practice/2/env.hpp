#ifndef ENV_HPP_
#define ENV_HPP_

#include <random>

#include "../case/publisher.hpp"
#include "../case/time.hpp"
#include "data.hpp"

class Env
    : public Subscriber<Time>,
      public Publisher<HttpRequest> {

    std::default_random_engine &random_engine;
    std::poisson_distribution<size_t> random_interval;
    std::uniform_int_distribution<size_t> random_passenger, random_flight;

    real_t next_request_time = 0;

  public:
    Env(std::default_random_engine &);

    void update(Time) override;
};

#endif
