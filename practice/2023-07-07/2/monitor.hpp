#pragma once

#include "../../../mocc/math.hpp"
#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <unordered_map>

class Monitor : public Recorder<StopwatchElapsedTime>,
                public Observer<Job>,
                public Observer<TaskDone> {

    std::unordered_map<size_t, real_t> jobs_start_time;

  public:
    OnlineDataAnalysis comp_time_data;

    void update(Job j) { jobs_start_time[j] = record; }

    void update(TaskDone t) {
        size_t v = ((Task)t).v;
        comp_time_data.insertDataPoint(record - jobs_start_time[v]);
        jobs_start_time.erase(v);
    }
};
