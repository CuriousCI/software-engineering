#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<Request>,
                public Recorder<class T> {
  public:
    real_t last_request_time = 0;
    bool preserves_order = true;

    void update(Request r) override {
        if (r.t < last_request_time)
            preserves_order = false;

        last_request_time = r.t;
    }
};

#endif
