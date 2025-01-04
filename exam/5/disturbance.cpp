#include "disturbance.hpp"

Disturbance::Disturbance(std::default_random_engine &random_engine)
    : random_engine(random_engine) {}

void Disturbance::update(Time time) {
    if (time < next_sampling)
        return;

    next_sampling = time + Td;
    notifySubscribers(random_disturbance(random_engine));
}
