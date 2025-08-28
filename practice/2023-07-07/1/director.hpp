#pragma once

#include "../../../mocc/math.hpp"
#include "../../../mocc/notifier.hpp"
#include "parameters.hpp"

class Director : public Observer<TaskDoneTime, EmployeeCost>,
                 public Notifier<ProjInitTime> {

    size_t completed_tasks = 0;
    real_t project_init_time = 0, project_cost = 0;

  public:
    OnlineDataAnalysis project_time_analysis, project_cost_analysis;

    void update(
        TaskDoneTime task_completion_time,
        EmployeeCost employee_daily_cost
    ) override {
        completed_tasks++;
        project_cost +=
            (task_completion_time - project_init_time) * employee_daily_cost;

        if (completed_tasks < observers.size())
            return;

        project_time_analysis.insertDataPoint(
            task_completion_time - project_init_time
        );
        project_cost_analysis.insertDataPoint(project_cost);

        project_init_time = task_completion_time;
        project_cost = 0;
        completed_tasks = 0;
        notify(project_init_time);
    }
};
