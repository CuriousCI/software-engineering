#include <cmath>
#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/math.hpp"
#include "../../../mocc/mocc.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include "uav.hpp"
#include <random>

int main() {
    {
        std::ifstream parameters("parameters.txt");
        char line_type;
        parameters >> line_type >> T >> line_type >> H >> line_type >> M >>
            line_type >> N >> line_type >> L >> line_type >> V >> line_type >>
            A >> line_type >> D;
        parameters.close();

        random_position = std::uniform_real_distribution<real_t>(-L, L);
    }

    OnlineDataAnalysis collisions_data;
    for (size_t simulation = 0; simulation < M; simulation++) {
        System system;
        Stopwatch stopwatch(T);

        system.addObserver(&stopwatch);
        std::vector<UAV *> uavs;
        for (size_t _ = 0; _ < N; _++) {
            UAV *uav = new UAV();

            uavs.push_back(uav);
            system.addObserver(uav);
        }

        size_t collisions = 0;
        while (stopwatch.elapsedTime() <= H) {
            system.next();

            for (size_t i = 0; i < N; i++) {
                for (size_t j = i + 1; j < N; j++) {
                    real_t distance = 0;
                    for (size_t k = 0; k < 3; k++) {
                        distance += pow(uavs[i]->x[k] - uavs[j]->x[k], 2);
                    }
                    distance = sqrt(distance);

                    if (distance <= D) {
                        collisions++;
                    }
                }
            }
        }

        collisions_data.insertDataPoint((real_t)collisions / H);
    }

    std::ofstream("results.txt") << "2025-01-09\nC " << collisions_data.mean();
    return 0;
}
