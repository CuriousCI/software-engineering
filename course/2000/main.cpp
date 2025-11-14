#include <iostream>

#include "../software.hpp"

int main() {
    enum LightColor { GREEN = 0, YELLOW = 1, RED = 2 };
    const size_t HORIZON = 1000;
    std::uniform_int_distribution<> random_time_interval(
        60, 120
    );
    LightColor light = LightColor::RED;
    size_t next_switch_time = random_time_interval(urng);

    for (size_t time = 0; time <= HORIZON; time++) {
        std::cout << time << ' ' << next_switch_time - time
                  << ' ' << light << std::endl;
        if (time < next_switch_time) {
            continue;
        }
        light =
            (light == RED ? GREEN
                          : (light == GREEN ? YELLOW : RED));
        next_switch_time = time + random_time_interval(urng);
    }

    return EXIT_SUCCESS;
}
