#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/mocc.hpp"
#include <random>

int main() {
    std::random_device random_device;
    urng_t urng(random_device());

    size_t N, C;
    const size_t ITERATIONS = 1000;

    std::vector<std::discrete_distribution<>> transition_matrix;
    std::vector<std::vector<real_t>> cost;

    {
        std::ifstream parameters("parameters.txt");

        char line_type;
        std::vector<std::pair<size_t, size_t>> arrows;
        std::vector<std::pair<real_t, real_t>> labels;

        while (parameters >> line_type)
            switch (line_type) {
            case 'C':
                parameters >> C;
                break;
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
                std::discrete_distribution<>(state.begin(), state.end())
            );
    }

    size_t cheap_iterations_number = 0;
    for (size_t _ = 0; _ < ITERATIONS; _++) {
        size_t current_state = 0;
        real_t project_cost = 0;

        while (current_state != N - 1) {
            size_t next_state = transition_matrix[current_state](urng);
            project_cost += cost[current_state][next_state];
            current_state = next_state;
        }

        if (project_cost <= C)
            cheap_iterations_number++;
    }

    std::ofstream("results.txt")
        << "2025-01-09\nP "
        << (real_t)cheap_iterations_number / (real_t)ITERATIONS;

    return 0;
}
