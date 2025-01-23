#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../../mocc/observer.hpp"
#include "../../mocc/recorder.hpp"
#include "../../mocc/stat.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"
#include <unordered_map>

class Monitor : public Recorder<T>,
                public Observer<Job>,
                public Observer<TaskDone> {

    std::unordered_map<size_t, real_t> jobs_start_time;

  public:
    Stat comp_time_stat;

    void update(Job j) { jobs_start_time[j] = record; }

    void update(TaskDone t) {
        size_t v = ((Task)t).v;
        comp_time_stat.save(record - jobs_start_time[v]);
        jobs_start_time.erase(v);
    }
};

#endif
