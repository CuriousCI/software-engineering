#include "env.hpp"

Env::Env(
    std::default_random_engine &random_engine,
    real_t tau,
    size_t N,
    size_t F
) :
    random_engine(random_engine),
    random_interval(tau),
    random_passenger(1, N),
    random_flight(1, F) {
}

void Env::update(Time time) {
    if (time < next_request_time)
        return;

    notifySubscribers(Data{
        random_passenger(random_engine),
        random_flight(random_engine),
        time
    });

    next_request_time = time + random_interval(random_engine);
}
