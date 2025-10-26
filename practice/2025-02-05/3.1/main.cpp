#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "../../../mocc/math.hpp"
#include "parameters.hpp"

int main() {
    std::vector<std::vector<real_t>> matrix(3, std::vector<real_t>(3, 0));

    {
        std::ifstream parameters("parameters.txt");

        for (size_t _ = 0; _ < 9; _++) {
            size_t i, j;
            real_t p;
            parameters >> i >> j >> p;
            matrix[i][j] = p;
            std::cout << i << " " << j << " " << p << std::endl;
        }

        parameters >> T1 >> T2;
    }

    std::vector<std::discrete_distribution<>> transition_matrix;

    for (auto &state : matrix) {
        transition_matrix.push_back(
            std::discrete_distribution<>(state.begin(), state.end())
        );
    }

    for (auto &line : transition_matrix) {
        for (auto &probability : line.probabilities()) {
            std::cout << probability << " ";
        }
        std::cout << std::endl;
    }

    OnlineDataAnalysis data;
    for (size_t _ = 0; _ < 1000; _++) {
        size_t customers = 0;
        real_t time = 0;

        size_t state = 0;
        while (time <= H) {
            size_t next_state = transition_matrix[state](urng);

            if (state == 0 && next_state == 1) {
                time += T1;
            }

            if (state == 0 && next_state == 2) {
                time += T2;
            }

            if (next_state == 0) {
                customers++;
            }

            state = next_state;
        }

        data.insertDataPoint((real_t)customers / (real_t)H);
    }

    std::cout << data.mean() << std::endl;
}

// real_t to_add = 0;
// if (requested1) {
//     to_add = T1;
// }
// if (requested2 && T2 > T1) {
//     to_add = T2;
// }
// requested1 = false;
// requested2 = false;
// time += to_add;
// bool requested1 = false;
// bool requested2 = false;
// requested2 = true;
// requested1 = true;
