#include "time.hpp"

Stopwatch::Stopwatch(real_t delta)
    : delta(delta) {}

void Stopwatch::tick() {
    elapsed_ += delta;
    notify(elapsed_);
}

real_t Stopwatch::elapsed() {
    return elapsed_;
}
