#ifndef PLANT_HPP_
#define PLANT_HPP_

#include "../case/reader.hpp"
#include "../case/time.hpp"
#include "data.hpp"

class Plant
    : public Reader<Ctrl>,
      public Reader<Dist>,
      public Subscriber<Time>,
      public Publisher<Temp> {

    real_t x = 0;

  public:
    void update(Time) override;
};

#endif
