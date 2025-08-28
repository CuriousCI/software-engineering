#pragma once

#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<CustomerPurchaseRequest>,
                public Recorder<StopwatchElapsedTime> {
  public:
    real_t last_request_time = 0;
    bool preserves_order = true;

    void update(CustomerPurchaseRequest request) override {
        if (request.time < last_request_time)
            preserves_order = false;

        last_request_time = request.time;
    }
};
