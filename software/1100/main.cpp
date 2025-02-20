#include <fstream>
#include <random>

#include "../../mocc/mocc.hpp"

const size_t HORIZON = 10;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    std::vector<real_t> state(2, 0);
    std::ofstream file("logs");

    for (size_t time = 0; time <= HORIZON; time++) {
        for (auto &r_i : state)
            r_i = uniform(urng);

        file << time << ' ';
        for (auto r_i : state)
            file << r_i << ' ';
        file << std::endl;
    }

    file.close();
    return 0;
}
