#pragma once

#include "../../../mocc/math.hpp"
#include "../../../mocc/mocc.hpp"
#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<PurchaseRequest>,
                public Recorder<StopwatchElapsedTime> {
    real_t last_request_time = 0;

  public:
    Data intervals_data;

    void update(PurchaseRequest) {
        intervals_data.insertDataPoint(record - last_request_time);
        last_request_time = record;
    }
};
