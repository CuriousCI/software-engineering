#include <fstream>
#include <random>

using real_t = double;
const size_t HORIZON = 100;

struct DTMC {
    real_t state[2];
};

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    std::ofstream log("log");
    std::vector<DTMC> dtmcs(2, {0, 0});

    for (size_t time = 0; time <= HORIZON; time++) {
        for (size_t r = 0; r < 2; r++) {
            dtmcs[0].state[r] =
                dtmcs[1].state[r] * uniform(random_engine);
            dtmcs[1].state[r] =
                dtmcs[0].state[r] + uniform(random_engine);
        }

        log << time << ' ';
        for (auto dtmc : dtmcs)
            for (auto r : dtmc.state)
                log << r << ' ';
        log << std::endl;
    }

    log.close();
    return 0;
}
