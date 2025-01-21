#ifndef PROJ_HPP_
#define PROJ_HPP_

#include "../case/publisher.hpp"
#include "../case/stat.hpp"
#include "data.hpp"

class Project
    : public Subscriber<TeamCost, TaskCompTime>,
      public Publisher<ProjInitTime> {

    size_t completed_tasks = 0;
    real_t init_time = 0, proj_cost = 0;

  public:
    Stat time, cost;

    void update(TeamCost, TaskCompTime) override;
};

#endif
