#include <iostream>
#include <random>

int main() {
    std::random_device random_device;
    std::default_random_engine rand_engine(random_device());
    std::uniform_int_distribution<> rand_cost(300, 1000);

    const size_t ITERATIONS = 10000;
    size_t below_550 = 0;

    for (size_t i = 0; i < ITERATIONS; i++)
        if (rand_cost(rand_engine) < 550)
            below_550++;

    std::cout << (double)below_550 / ITERATIONS << std::endl;

    return 0;
}
