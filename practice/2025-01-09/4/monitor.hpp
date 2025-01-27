#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../../../mocc/observer.hpp"
#include "../../../mocc/recorder.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Monitor : public Observer<CustomerId, RequestCount>,
                public Recorder<class T> {

    std::vector<size_t> counts;

  public:
    Monitor(size_t N) : counts(N) {}

    size_t total_requests = 0;

    void update(CustomerId i, RequestCount c) override {
        total_requests -= counts[i - 1];
        counts[i - 1] = c;
        total_requests += counts[i - 1];
    }
};

#endif
