#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../case/stat.hpp"
#include "../case/subscriber.hpp"
#include "data.hpp"

class Monitor : public Subscriber<HttpRequest> {
    real_t last_request_time = 0;

  public:
    Stat interval;

    void update(HttpRequest);
};

#endif
