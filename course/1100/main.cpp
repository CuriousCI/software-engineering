#include <iostream>

#include "../software.hpp"

int main() {
    std::uniform_real_distribution<> random_uniform(0, 1);
    std::vector<real_t> MDP_state_vector(2, 0);

    const size_t HORIZON = 10;

    for (size_t time = 0; time <= HORIZON; time++) {
        std::cout << time << ' ';
        for (real_t &component : MDP_state_vector) {
            component = random_uniform(urng);
            std::cout << component << ' ';
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
