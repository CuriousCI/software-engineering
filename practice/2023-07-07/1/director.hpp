#pragma once

#include "../../../mocc/math.hpp"
#include "../../../mocc/notifier.hpp"
#include "parameters.hpp"

class Director : public Observer<TaskDoneTime, EmployeeCost>,
                 public Notifier<ProjInitTime> {

    size_t completed_tasks = 0;
    real_t proj_init = 0, proj_cost = 0;

  public:
    Data time_data, cost_data;

    void update(TaskDoneTime task_completion_time,
                EmployeeCost employee_daily_cost) override {

        completed_tasks++;
        proj_cost += (task_completion_time - proj_init) * employee_daily_cost;

        if (completed_tasks < observers.size())
            return;

        time_data.insertDataPoint(task_completion_time - proj_init);
        cost_data.insertDataPoint(proj_cost);

        proj_init = task_completion_time;
        proj_cost = 0;
        completed_tasks = 0;
        notify(proj_init);
    }
};
