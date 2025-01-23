#include "../../mocc/system.hpp"
#include "../../mocc/time.hpp"
#include "delivery.hpp"
#include "dispatcher.hpp"
#include "generator.hpp"
#include "monitor.hpp"
#include "parameters.hpp"
#include "worker.hpp"
#include <fstream>
#include <iostream>
#include <vector>

/* 07/07/2023 - ex1 */

int main() {
    {
        std::ifstream params("parameters.txt");
        char c;

        // clang-format off
        while (params >> c)
            switch (c) {
            case 'W': params >> W; break;
            case 'N': params >> N; break;
            case 'B': params >> B; break;
            case 'L': params >> L; break;
            case 'D': params >> D; break;
            }
        // clang-format on

        params.close();
    }

    /* Setup */
    std::random_device random_device;
    urng_t urng(random_device());

    System system;
    Stopwatch stopwatch;
    Generator generator(urng);
    Dispatcher dispatcher(urng, &system);
    Delivery delivery(&system);
    Monitor monitor;
    std::vector<Worker *> workers;

    for (size_t i = 1; i <= W; i++) {
    }

    system.addObserver(&stopwatch);
    stopwatch.addObserver(&generator);

    while (stopwatch.elapsed() < HORIZON)
        try {
            system.next();
        } catch (buffer_full e) {
            break;
        }

    {
        // clang-format off
        // clang-format on
    }

    return 0;
}
