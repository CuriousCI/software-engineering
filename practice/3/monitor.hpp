#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../case/reader.hpp"
#include "../case/stat.hpp"
#include "../case/subscriber.hpp"
#include "../case/time.hpp"
#include "data.hpp"

class Monitor
    : public Reader<Time>,
      public Subscriber<HttpResponse> {

  public:
    const size_t id;
    Stat response_time;

    Monitor(const size_t id);

    void update(HttpResponse) override;
};

#endif
