#include <fstream>
#include <random>

using real_t = double;
const size_t HORIZON = 10;

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    std::vector<real_t> state(2, 0);

    std::ofstream log("log");

    for (size_t time = 0; time <= HORIZON; time++) {
        for (auto &r : state)
            r = uniform(random_engine);

        log << time << ' ';
        for (auto r : state)
            log << r << ' ';
        log << std::endl;
    }

    log.close();
    return 0;
}
