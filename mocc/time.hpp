#pragma once

#include "alias.hpp"
#include "mocc.hpp"
#include "notifier.hpp"
#include "system.hpp"

STRONG_ALIAS(StopwatchElapsedTime, real_t)

/* docs.rs/bevy/latest/bevy/time/struct.Stopwatch.html
 * An object that measures time. It is a SystemObserver, so it must be attached
 * to a System for it to measure the time elapsed in the systme.
 *
 * System system.
 * Stopwatch stopwatch.
 *
 * system.addObserver(&stopwatch);
 * system.next();
 * system.next();
 *
 * stopwatch.elapsedTime(); // 2
 * */
class Stopwatch : public SystemObserver, public Notifier<StopwatchElapsedTime> {
  private:
    real_t elapsed_time = 0;
    const real_t time_step;

  public:
    Stopwatch(real_t time_step = 1);

    /* Returns the "elapsed time" since the Stopwatch was connected to the
     * system or was reset.
     */
    real_t elapsedTime();

    /* Resets the "elapsed time" to 0. */
    void reset();

    /* Synchronizes to a system. */
    void update() override;
};

STRONG_ALIAS(TimerEnded, real_t)

enum class TimerMode {
    /* When the timer ends it doesn't restart. */
    Once,

    /* When the timer ends it automatically restarts. */
    Repeating
};

/* docs.rs/bevy/latest/bevy/time/struct.Timer.html */
/* A timer is synchronized to a system.
 * It updates the elapsed time as the system is simulated, until the duration of
 * the timer is over.
 * */
class Timer : public SystemObserver, public Notifier<TimerEnded> {
  private:
    real_t duration, elapsed_time = 0;
    bool is_finished = false;
    const real_t time_step;
    TimerMode mode;

  public:
    Timer(real_t duration, TimerMode mode, real_t time_step);

    /* Resets the timer with a new initial duration. */
    void resetWithDuration(real_t duration);

    /* Synchronizes to a system. */
    void update() override;
};

/* Many entities are slower than the system's simulation speed. These entities
 * need to be connected to a timer.
 *
 * This is a boilerplate class which proves to be useful quite often.
 * */
class TimerBasedEntity : public Observer<TimerEnded> {
  protected:
    /* An timer based entity has access to the timer it is connected to.
     * It can call resetWithDuration() if the timer is not repeating.
     * */
    Timer timer;

  public:
    TimerBasedEntity(
        System &system,
        real_t duration,
        TimerMode mode,
        real_t time_step
    )
        : timer(duration, mode, time_step) {
        system.addObserver(&timer);
        timer.addObserver(this);
    }
};
