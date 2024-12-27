#include <fstream>
#include <random>

using real_t = double;
const size_t HORIZON = 1000;

enum Light { GREEN = 0, YELLOW = 1, RED = 2 };

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    auto random_timer_duration =
        std::uniform_int_distribution<>(60, 120);
    std::ofstream log("log");

    Light traffic_light = Light::RED;
    size_t timer = random_timer_duration(random_engine);

    for (size_t time = 0; time <= HORIZON; time++) {
        log << time << ' ' << timer << ' ' << traffic_light
            << std::endl;

        if (timer > 0) {
            timer--;
            continue;
        }

        traffic_light =
            (traffic_light == RED
                 ? GREEN
                 : (traffic_light == GREEN ? YELLOW : RED));
        timer = random_timer_duration(random_engine);
    }

    log.close();
    return 0;
}
