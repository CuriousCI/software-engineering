#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../../../mocc/math.hpp"
#include "../../../mocc/mocc.hpp"
#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<Request>,
                public Recorder<class T> {

    real_t last_request_time = 0;

  public:
    Stat interval_stat;

    void update(Request _) {
        interval_stat.save(record - last_request_time);
        last_request_time = record;
    }
};

#endif
