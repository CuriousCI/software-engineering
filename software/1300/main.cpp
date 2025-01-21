#include <fstream>
#include <vector>

using real_t = double;
const size_t HORIZON = 100;

struct MDP {
    real_t state[2];
};

int main() {
    std::ofstream log("log");
    std::vector<MDP> mdps({{2, 2}, {1, 1}});

    for (size_t time = 0; time <= HORIZON; time++) {
        mdps[0].state[0] =
            .7 * mdps[0].state[0] + .7 * mdps[0].state[1];
        mdps[0].state[1] =
            -.7 * mdps[0].state[0] + .7 * mdps[0].state[1];

        mdps[1].state[0] = mdps[1].state[0] + mdps[1].state[1];
        mdps[1].state[1] =
            -mdps[1].state[0] + mdps[1].state[1];

        log << time << ' ';
        for (auto mdp : mdps)
            for (auto r : mdp.state)
                log << r << ' ';
        log << std::endl;
    }

    log.close();
    return 0;
}
