#include <iostream>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::uniform_int_distribution<> random_delta(20, 30);
    size_t delta = random_delta(pseudo_random_engine),
           next_send_time = delta;

    for (size_t time = 0; time < SIMULATION_HORIZON; time++) {
        if (time < next_send_time) {
            continue;
        }

        std::cout << delta << std::endl; /* v_i */
        delta = random_delta(pseudo_random_engine);
        next_send_time = time + delta;
    }

    return EXIT_SUCCESS;
}
