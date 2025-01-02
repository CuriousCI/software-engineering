#include <iostream>
#include <random>

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    std::discrete_distribution<> transition_matrix[] = {
        {0, 1},
        {0, .3, .7},
        {0, .2, .2, .6},
        {0, .1, .2, .1, .6},
        {1},
    };

    size_t state = 0;
    for (size_t step = 0; step < 15; step++) {
        state = transition_matrix[state](random_engine);
        std::cout << state << std::endl;
    }

    return 0;
}
