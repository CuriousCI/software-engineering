#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/math.hpp"
#include "../../../mocc/mocc.hpp"
#include <random>

int main() {
    std::random_device random_device;
    urng_t urng(random_device());

    const size_t ITERATIONS = 1000;

    size_t N;
    std::vector<std::discrete_distribution<>> transition_matrix;
    std::vector<std::vector<real_t>> cost;

    {
        std::ifstream parameters("parameters.txt");

        char A;
        std::vector<std::pair<size_t, size_t>> arrows;
        std::vector<std::pair<real_t, real_t>> labels;

        while (parameters >> A)
            switch (A) {
            case 'N':
                parameters >> N;
                break;
            case 'A':
                size_t i, j;
                real_t P, C;
                parameters >> i >> j >> P >> C;
                arrows.push_back({i, j});
                labels.push_back({P, C});
                break;
            }

        parameters.close();

        cost = std::vector<std::vector<real_t>>(N, std::vector<real_t>(N, 0));
        std::vector<std::vector<real_t>> matrix(N, std::vector<real_t>(N, 0));
        matrix[N - 1][N - 1] = 1;

        for (size_t i = 0; i < arrows.size(); i++) {
            matrix[arrows[i].first][arrows[i].second] = labels[i].first;
            cost[arrows[i].first][arrows[i].second] = labels[i].second;
        }

        for (auto &state : matrix)
            transition_matrix.push_back(
                std::discrete_distribution<>(state.begin(), state.end()));
    }

    Data proj_cost_data;
    for (size_t _ = 0; _ < ITERATIONS; _++) {
        size_t curr_state = 0;
        real_t proj_cost = 0;

        while (curr_state != N - 1) {
            size_t next_state = transition_matrix[curr_state](urng);
            proj_cost += cost[curr_state][next_state];
            curr_state = next_state;
        }

        proj_cost_data.insertDataPoint(proj_cost);
    }

    std::ofstream("results.txt") << "2025-01-09\nC " << proj_cost_data.mean();

    return 0;
}
