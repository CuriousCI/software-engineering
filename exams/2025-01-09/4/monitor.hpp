#pragma once

#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<CustomerId, RequestsCount>,
                public Recorder<StopwatchElapsedTime> {

    std::vector<size_t> counts;

  public:
    size_t total_requests_received = 0;

    Monitor(size_t N) : counts(N) {}

    void update(CustomerId customer_id, RequestsCount requests_count) override {
        total_requests_received -= counts[customer_id - 1];
        counts[customer_id - 1] = requests_count;
        total_requests_received += counts[customer_id - 1];
    }
};
