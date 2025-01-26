#ifndef MOCC_TIME_HPP_
#define MOCC_TIME_HPP_

#include "alias.hpp"
#include "mocc.hpp"
#include "notifier.hpp"

ALIAS_TYPE(T, real_t)

/* docs.rs/bevy/latest/bevy/time/struct.Stopwatch.html */
class Stopwatch : public Observer<>, public Notifier<T> {
    real_t elapsed_ = 0;
    const real_t delta;

  public:
    Stopwatch(real_t delta = 1);

    real_t elapsed();
    void update() override;
};

/*ALIAS_TYPE(U, real_t)*/
enum class TimerMode { Once, Repeating };

/* docs.rs/bevy/latest/bevy/time/struct.Timer.html */
class Timer : public Observer<>, public Notifier<> {
    real_t duration, elapsed = 0;
    bool finished = false;
    const real_t delta;
    TimerMode mode;

  public:
    Timer(real_t duration, TimerMode mode, real_t delta = 1);

    void set_duration(real_t time);
    void update() override;
};

#endif
