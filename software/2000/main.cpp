#include <fstream>
#include <random>

using real_t = double;
using urng_t = std::default_random_engine;
const size_t HORIZON = 1000;

enum Light { GREEN = 0, YELLOW = 1, RED = 2 };

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::uniform_int_distribution<> random_interval(60, 120);
    std::ofstream log("log");

    Light traffic_light = Light::RED;
    size_t next_switch = random_interval(urng);

    for (size_t time = 0; time <= HORIZON; time++) {
        log << time << ' ' << next_switch - time << ' '
            << traffic_light << std::endl;

        if (time < next_switch)
            continue;

        traffic_light =
            (traffic_light == RED
                 ? GREEN
                 : (traffic_light == GREEN ? YELLOW : RED));

        next_switch = time + random_interval(urng);
    }

    log.close();
    return 0;
}
