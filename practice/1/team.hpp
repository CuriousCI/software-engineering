#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <random>

#include "../case/stat.hpp"
#include "../case/time.hpp"
#include "data.hpp"

class Team
    : public Subscriber<Time>,
      public Subscriber<ProjInitTime>,
      public Publisher<TeamCost, TaskCompTime> {

    std::default_random_engine &random_engine;
    std::vector<std::discrete_distribution<>> transition_matrix;
    size_t phase = 0;
    real_t proj_init_time = 0;

  public:
    const size_t id;
    const real_t cost;
    Stat completion_time;

    Team(std::default_random_engine &, size_t k);

    void update(ProjInitTime) override;
    void update(Time) override;
};

#endif
