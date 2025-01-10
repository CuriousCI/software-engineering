#include <iostream>
#include <random>

using urng_t = std::default_random_engine;

float average(std::vector<float> X) {
    float sum = 0;
    for (auto x_i : X)
        sum += x_i;

    return sum / X.size();
}

float average_r(std::vector<float> X) {
    float average = 0;
    for (size_t n = 0; n < X.size(); n++)
        average =
            average * ((float)n / (n + 1)) + X[n] / (n + 1);

    return average;
}

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_real_distribution<float> x_i(0, 10e34);

    std::vector<float> X;

    for (size_t _ = 0; _ < 10e3; _++)
        X.push_back(x_i(urng));

    std::cout << average(X) << std::endl
              << average_r(X) << std::endl;

    return 0;
}
