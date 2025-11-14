#include "time.hpp"

Stopwatch::Stopwatch(real_t time_step) : time_step(time_step) {}

real_t Stopwatch::elapsedTime() { return elapsed_time; }

void Stopwatch::reset() { this->elapsed_time = 0; }

void Stopwatch::update() {
    elapsed_time += time_step;
    notify(elapsed_time);
}

Timer::Timer(real_t duration, TimerMode mode, real_t time_step)
    : duration(duration), mode(mode), time_step(time_step) {}

void Timer::resetWithDuration(real_t duration) {
    this->duration = duration;
    this->elapsed_time = 0;
    this->is_finished = false;
}

void Timer::update() {
    if (elapsed_time < duration) {
        elapsed_time += time_step;
    }

    if (elapsed_time + time_step >= duration && !is_finished) {
        switch (mode) {
        case TimerMode::Repeating:
            elapsed_time = 0;
            break;
        case TimerMode::Once:
            is_finished = true;
            break;
        }

        notify(elapsed_time);
    }
}
