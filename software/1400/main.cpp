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
    std::vector<DTMC> dtmcs({{1, 1}, {2, 2}});

    for (size_t time = 0; time <= HORIZON; time++) {
        dtmcs[0].state[0] =
            .7 * dtmcs[0].state[0] + .7 * dtmcs[0].state[1];
        dtmcs[0].state[1] =
            -.7 * dtmcs[0].state[0] + .7 * dtmcs[0].state[1];

        dtmcs[1].state[0] =
            dtmcs[1].state[0] + dtmcs[1].state[1];
        dtmcs[1].state[1] =
            -dtmcs[1].state[0] + dtmcs[1].state[1];

        log << time << ' ';
        for (auto dtmc : dtmcs)
            for (auto r : dtmc.state)
                log << r << ' ';
        log << std::endl;
    }

    log.close();
    return 0;
}
