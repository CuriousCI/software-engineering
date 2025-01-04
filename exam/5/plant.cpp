#include "plant.hpp"

void Plant::update(Time time) {
    real_t u = Reader<Ctrl>::value, d = Reader<Dist>::value;
    x += a * std::min(1.0, std::max(0.0, u)) - b - d;
    notifySubscribers(x);
}
