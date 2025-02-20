#include <fstream>
#include <random>

#include "../../mocc/mocc.hpp"

const size_t HORIZON = 10;

struct MDP {
    real_t state[2];
};

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    std::vector<MDP> mdps(2, {0, 0});
    std::ofstream file("logs");

    for (size_t time = 0; time <= HORIZON; time++) {
        for (size_t r = 0; r < 2; r++) {
            mdps[0].state[r] =
                mdps[1].state[r] * uniform(urng);
            mdps[1].state[r] =
                mdps[0].state[r] + uniform(urng);
        }

        file << time << ' ';
        for (auto mdp : mdps)
            for (auto r_i : mdp.state)
                file << r_i << ' ';
        file << std::endl;
    }

    file.close();
    return 0;
}
