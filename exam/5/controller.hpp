#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include "../case/publisher.hpp"
#include "../case/reader.hpp"
#include "data.hpp"

class Controller
    : public Reader<TempSetPoint>,
      public Subscriber<Temp>,
      public Publisher<Ctrl> {

    real_t x = 0, x1 = 0;

  public:
    void update(class Temp) override;
};

#endif
