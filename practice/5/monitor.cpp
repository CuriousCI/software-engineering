#include "monitor.hpp"

void Monitor::update(Temp heat) {
    err.save(heat - Reader<TempSetPoint>::value);
}
