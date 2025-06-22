#pragma once

#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<CustomerId, RequestsCount>,
                public Recorder<StopwatchElapsedTime> {

    std::vector<size_t> counts;

  public:
    size_t total_requests = 0;

    Monitor(size_t N) : counts(N) {}

    void update(CustomerId id, RequestsCount count) override {
        total_requests -= counts[id - 1];
        counts[id - 1] = count;
        total_requests += counts[id - 1];
    }
};
