#include <iostream>

#include "listings.hpp"

float traditional_mean(std::vector<float> values) {
    float values_sum =
        std::accumulate(values.begin(), values.end(), 0);

    return values_sum / (float)values.size();
}

float incremental_mean(std::vector<float> values) {
    float mean = 0;
    for (size_t n = 0; n < values.size(); n++) {
        mean =
            mean * ((float)n / (n + 1)) + values[n] / (n + 1);
    }

    return mean;
}

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::uniform_real_distribution<float> random_value(
        0, 10e34
    );

    std::vector<float> values;
    for (size_t _ = 0; _ < 10e3; _++) {
        values.push_back(random_value(pseudo_random_engine));
    }

    std::cout << traditional_mean(values) << std::endl
              << incremental_mean(values) << std::endl;

    return EXIT_SUCCESS;
}
