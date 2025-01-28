#include <iomanip>
#include <iostream>
#include <map>
#include <random>

using urng_t = std::default_random_engine;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::normal_distribution<> normal(12, 2);

    std::map<long, unsigned> histogram{};
    for (size_t i = 0; i < 10000; i++)
        ++histogram[(size_t)normal(urng)];

    for (const auto [k, v] : histogram)
        if (v / 200 > 0)
            std::cout << std::setw(2) << k << ' '
                      << std::string(v / 200, '*') << '\n';

    return 0;
}
