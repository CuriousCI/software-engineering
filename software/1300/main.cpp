#include <fstream>
#include <vector>

#include "../../mocc/mocc.hpp"

const size_t HORIZON = 10;

struct MDP {
    real_t state[2];
};

int main() {
    std::vector<MDP> mdps({{2, 2}, {1, 1}});
    std::ofstream file("logs");

    for (size_t time = 0; time <= HORIZON; time++) {
        mdps[0].state[0] =
            .7 * mdps[0].state[0] + .7 * mdps[0].state[1];
        mdps[0].state[1] =
            -.7 * mdps[0].state[0] + .7 * mdps[0].state[1];

        mdps[1].state[0] = mdps[1].state[0] + mdps[1].state[1];
        mdps[1].state[1] =
            -mdps[1].state[0] + mdps[1].state[1];

        file << time << ' ';
        for (auto mdp : mdps)
            for (auto r_i : mdp.state)
                file << r_i << ' ';
        file << std::endl;
    }

    file.close();
    return 0;
}
