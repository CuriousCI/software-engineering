#include <iostream>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::uniform_int_distribution<> random_T(20, 30);

    size_t T = random_T(pseudo_random_engine),
           next_request_time = T;

    for (size_t time = 0; time < SIMULATION_HORIZON; time++) {
        if (time < next_request_time) {
            continue;
        }

        std::cout << T << std::endl;
        T = random_T(pseudo_random_engine);
        next_request_time = time + T;
    }

    return EXIT_SUCCESS;
}
