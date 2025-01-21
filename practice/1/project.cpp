#include "project.hpp"

void Project::update(TeamCost team_cost, TaskCompTime task_comp_time) {
    completed_tasks++;
    proj_cost += (task_comp_time - init_time) * team_cost;

    if (completed_tasks < subscribers.size())
        return;

    time.save(task_comp_time - init_time);
    cost.save(proj_cost);

    init_time = task_comp_time;
    proj_cost = 0;
    completed_tasks = 0;
    notifySubscribers(init_time);
}
