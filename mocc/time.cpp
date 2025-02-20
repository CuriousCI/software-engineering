#include "time.hpp"

Stopwatch::Stopwatch(real_t delta) : delta(delta) {}

void Stopwatch::update() {
    elapsed_ += delta;
    notify(elapsed_);
}

real_t Stopwatch::elapsed() { return elapsed_; }

Timer::Timer(real_t duration, TimerMode mode, real_t delta)
    : duration(duration), mode(mode), delta(delta) {}

void Timer::set_duration(real_t duration) {
    this->duration = duration;
    this->elapsed = 0;
    this->finished = false;
}

void Timer::update() {
    if (elapsed < duration)
        elapsed += delta;
    else if (!finished) {
        switch (mode) {
        case TimerMode::Repeating:
            elapsed = 0;
            break;
        case TimerMode::Once:
            finished = true;
            break;
        }

        notify(elapsed);
    }
}
