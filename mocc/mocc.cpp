#include "mocc.hpp"

urng_t pseudo_random_engine_from_device() {
    std::random_device device_randomness_source;
    auto random_seed = device_randomness_source();
    urng_t pseudo_random_engine(random_seed);

    return pseudo_random_engine;
}
