#include <iomanip>
#include <iostream>
#include <map>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::poisson_distribution<> poisson(4);

    std::map<long, unsigned> histogram{};
    for (size_t _ = 0; _ < ITERATIONS; _++) {
        histogram[(size_t)poisson(pseudo_random_engine)]++;
    }

    for (const auto [k, v] : histogram) {
        if (v / 100 > 0) {
            std::cout << std::setw(2) << k << ' '
                      << std::string(v / 100, '*') << '\n';
        }
    }
}
