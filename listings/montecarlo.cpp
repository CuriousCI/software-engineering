#include <iostream>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::uniform_int_distribution<> features_cost_distribution(
        300, 1000
    );

    size_t number_of_iterations_less_than_550 = 0;

    for (size_t iteration = 0; iteration < ITERATIONS;
         iteration++) {
        if (features_cost_distribution(pseudo_random_engine) <
            550) {
            number_of_iterations_less_than_550++;
        }
    }

    std::cout << (double)number_of_iterations_less_than_550 /
                     (double)ITERATIONS
              << std::endl;

    return EXIT_SUCCESS;
}
