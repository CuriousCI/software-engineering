#pragma once

#include "../../../mocc/math.hpp"
#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<CustomerPurchaseRequest>,
                public Recorder<StopwatchElapsedTime> {
    real_t last_request_time = 0;

  public:
    OnlineDataAnalysis requests_interval_analysis;

    void update(CustomerPurchaseRequest) {
        requests_interval_analysis.insertDataPoint(record - last_request_time);
        last_request_time = record;
    }
};
