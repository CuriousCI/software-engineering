#include <iostream>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::exponential_distribution<> random_time_interval(
        5.0 / 60.0
    );

    float next_request_time = 0;
    size_t total_requests = 0, total_minutes = 0;
    for (size_t time = 0; time < SIMULATION_HORIZON; time++) {
        if (time % 60 == 0) {
            total_minutes++;
        }

        if (time < next_request_time) {
            continue;
        }

        total_requests++;
        next_request_time =
            time + random_time_interval(pseudo_random_engine);
    }

    /* Should be 5. */
    std::cout << (float)total_requests / (float)total_minutes
              << std::endl;

    return EXIT_SUCCESS;
}
