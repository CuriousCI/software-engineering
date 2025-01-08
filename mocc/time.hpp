#ifndef MOCC_TIME_HPP_
#define MOCC_TIME_HPP_

#include "alias.hpp"
#include "mocc.hpp"
#include "notifier.hpp"

ALIAS_TYPE(Time, real_t)

class Stopwatch : public Notifier<Time> {
    real_t elapsed_ = 0;
    const real_t delta;

  public:
    Stopwatch(real_t delta = 1);

    void tick();
    real_t elapsed();
};

#endif
