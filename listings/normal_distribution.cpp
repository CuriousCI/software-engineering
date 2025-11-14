#include <iomanip>
#include <iostream>
#include <map>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::normal_distribution<> normal_distribution(12, 2);

    std::map<long, unsigned> histogram{};
    for (size_t _ = 0; _ < ITERATIONS; _++) {
        histogram[(size_t)normal_distribution(
            pseudo_random_engine
        )]++;
    }

    for (const auto [k, v] : histogram) {
        if (v / 200 > 0) {
            std::cout << std::setw(2) << k << ' '
                      << std::string(v / 200, '*')
                      << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
