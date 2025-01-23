#ifndef DIRECTOR_HPP_
#define DIRECTOR_HPP_

#include "../../mocc/notifier.hpp"
#include "../../mocc/stat.hpp"
#include "parameters.hpp"

class Director : public Observer<TaskDone, EmplCost>,
                 public Notifier<ProjInit> {

    size_t completed_tasks = 0;
    real_t proj_init = 0, proj_cost = 0;

  public:
    Stat time_stat, cost_stat;

    void update(TaskDone task_completion_time,
                EmplCost employee_daily_cost) override {

        completed_tasks++;
        proj_cost += (task_completion_time - proj_init) *
                     employee_daily_cost;

        if (completed_tasks < observers.size())
            return;

        time_stat.save(task_completion_time - proj_init);
        cost_stat.save(proj_cost);

        proj_init = task_completion_time;
        proj_cost = 0;
        completed_tasks = 0;
        notify(proj_init);
    }
};

#endif
