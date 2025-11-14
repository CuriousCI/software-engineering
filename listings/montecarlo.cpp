#include <iostream>

#include "listings.hpp" /* EXPERIMENTS = 1000000 */

int main() {
    /* A random number generator (or random engine) is
     * required. This function is explained in detail in
     * Section 2.2.1. */
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::uniform_int_distribution<> rand_feature_cost(
        300, 1000
    );

    size_t number_of_experiments_less_than_550 = 0;
    for (size_t _ = 0; _ < EXPERIMENTS; _++) {
        /* Sample a random feature cost. */
        if (rand_feature_cost(pseudo_random_engine) < 550) {
            /* Yield 1 if the feature's cost is less than 550,
             * then add it to the numerator. */
            number_of_experiments_less_than_550++;
        }
    }

    /* Equation 11. */
    std::cout << (float)number_of_experiments_less_than_550 /
                     (float)EXPERIMENTS
              << std::endl;

    return EXIT_SUCCESS;
}
