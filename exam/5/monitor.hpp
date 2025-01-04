#ifndef MONITOR_HPP_
#define MONITOR_HPP_

#include "../case/reader.hpp"
#include "../case/stat.hpp"
#include "../case/subscriber.hpp"
#include "data.hpp"

class Monitor : public Reader<TempSetPoint>, public Subscriber<Temp> {
  public:
    Stat err;

    void update(Temp) override;
};

#endif
