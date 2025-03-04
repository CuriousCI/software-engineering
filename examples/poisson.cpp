#include <iomanip>
#include <iostream>
#include <map>
#include <random>

using urng_t = std::default_random_engine;
using real_t = float;
const size_t HORIZON = 10000;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::poisson_distribution<> poisson(4);

    std::map<long, unsigned> histogram{};
    for (size_t i = 0; i < 10000; i++)
        ++histogram[(size_t)poisson(urng)];

    for (const auto [k, v] : histogram)
        if (v / 100 > 0)
            std::cout << std::setw(2) << k << ' '
                      << std::string(v / 100, '*') << '\n';
}
