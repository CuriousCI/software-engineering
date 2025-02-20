#pragma once

#include "alias.hpp"
#include "mocc.hpp"
#include "notifier.hpp"
#include "system.hpp"

STRONG_ALIAS(T, real_t)

/* docs.rs/bevy/latest/bevy/time/struct.Stopwatch.html */
class Stopwatch : public Observer<>, public Notifier<T> {
    real_t elapsed_ = 0;
    const real_t delta;

  public:
    Stopwatch(real_t delta = 1);

    real_t elapsed();
    void update() override;
};

STRONG_ALIAS(U, real_t)
enum class TimerMode { Once, Repeating };

/* docs.rs/bevy/latest/bevy/time/struct.Timer.html */
class Timer : public Observer<>, public Notifier<U> {
    real_t duration, elapsed = 0;
    bool finished = false;
    const real_t delta;
    TimerMode mode;

  public:
    Timer(real_t duration, TimerMode mode, real_t delta = 1);

    void set_duration(real_t time);
    void update() override;
};

class Timed : public Observer<U> {
  protected:
    Timer timer;

  public:
    Timed(System *system, real_t duration, TimerMode mode,
          real_t delta = 1)
        : timer(duration, mode, delta) {
        system->addObserver(&timer);
        timer.addObserver(this);
    }
};
