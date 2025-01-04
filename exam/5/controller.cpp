#include "controller.hpp"

#include <cstdlib>

void Controller::update(Temp temp) {
    x = x1;
    x1 = temp;

    real_t u = k1 * (x - Reader<TempSetPoint>::value) + (k2) * (x1 - x);
    notifySubscribers(u);
}
