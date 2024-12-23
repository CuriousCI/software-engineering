#include <fstream>
#include <random>

typedef long double real_t;

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_real_distribution<real_t> uniform(0, 1);

    const size_t HORIZON = 10;
    std::vector<real_t> state(2, 0);

    std::ofstream output("output.csv");

    for (size_t time = 0; time <= HORIZON; time++) {
        for (auto &r : state)
            r = uniform(random_engine);

        // output << time << ' ';
        // for (auto r : state)
        //     output << r << ' ';
        // output << std::endl;
    }

    // output.close();

    return 0;
}
