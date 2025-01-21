#include <fstream>
#include <random>

using urng_t = std::default_random_engine;
using real_t = double;
const size_t HORIZON = 100;

struct MDP {
    real_t state[2];
};

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    std::ofstream log("log");
    std::vector<MDP> mdps(2, {0, 0});

    for (size_t time = 0; time <= HORIZON; time++) {
        for (size_t r = 0; r < 2; r++) {
            mdps[0].state[r] =
                mdps[1].state[r] * uniform(urng);
            mdps[1].state[r] =
                mdps[0].state[r] + uniform(urng);
        }

        log << time << ' ';
        for (auto dtmc : mdps)
            for (auto r : dtmc.state)
                log << r << ' ';
        log << std::endl;
    }

    log.close();
    return 0;
}
