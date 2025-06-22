#pragma once

#include "alias.hpp"
#include "mocc.hpp"
#include "notifier.hpp"
#include "system.hpp"

STRONG_ALIAS(StopwatchElapsedTime, real_t)

/* docs.rs/bevy/latest/bevy/time/struct.Stopwatch.html */
/* A stopwatch is synchronized to a system. */
/* It updates the elapsed time as the system is simulated.  */
class Stopwatch : public SystemObserver, public Notifier<StopwatchElapsedTime> {
  private:
    real_t elapsed_time = 0;
    const real_t time_step;

  public:
    Stopwatch(real_t time_step = 1);

    /* Elapsed time since the Stopwatch was connected to the system. */
    real_t elapsedTime();
    /* Synchronize to the system. */
    void update() override;
};

STRONG_ALIAS(TimerEnded, real_t)
enum class TimerMode {
    /* If the timer ends, the timer stops. */
    Once,
    /* If the timer ends, the timer automatically resets and restarts. */
    Repeating
};

/* docs.rs/bevy/latest/bevy/time/struct.Timer.html */
/* A timer is synchronized to a system. */
/* It updates the elapsed time as the system is simulated, until the duration of
 * the timer is over. */
class Timer : public SystemObserver, public Notifier<TimerEnded> {
  private:
    real_t duration, elapsed_time = 0;
    bool is_finished = false;
    const real_t time_step;
    TimerMode mode;

  public:
    Timer(real_t duration, TimerMode mode, real_t time_step = 1);

    /* Resets the timer with a new duration. */
    void resetWithDuration(real_t duration);
    /* Synchronize to the system. */
    void update() override;
};

/* Many entities are synchronized to a timer which is slower than the system. */
/* This is a boilerplate class which proved to be useful quite often. */
class TimerBasedEntity : public Observer<TimerEnded> {
  protected:
    /* An entity which timer based has access to the timer it follows. */
    /* It can call resetWithDuration() if the timer is not repeating. */
    Timer timer;

  public:
    TimerBasedEntity(System &system, real_t duration, TimerMode mode,
                     real_t delta = 1)
        : timer(duration, mode, delta) {
        system.addObserver(&timer);
        timer.addObserver(this);
    }
};
