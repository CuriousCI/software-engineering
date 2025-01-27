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
    std::vector<std::discrete_distribution<>>
        transition_matrix;
    std::vector<std::vector<real_t>> cost;

    {
        std::ifstream parameters("parameters.txt");

        char c;
        std::vector<std::pair<size_t, size_t>> arrows;
        std::vector<std::pair<real_t, real_t>> labels;

        while (parameters >> c)
            switch (c) {
            case 'N':
                parameters >> N;
                break;
            case 'A':
                size_t i, j;
                real_t p, c;
                parameters >> i >> j >> p >> c;
                arrows.push_back({i, j});
                labels.push_back({p, c});
                break;
            }

        parameters.close();

        cost = std::vector<std::vector<real_t>>(
            N, std::vector<real_t>(N, 0));
        std::vector<std::vector<real_t>> matrix(
            N, std::vector<real_t>(N, 0));
        matrix[N - 1][N - 1] = 1;

        for (size_t i = 0; i < arrows.size(); i++) {
            matrix[arrows[i].first][arrows[i].second] =
                labels[i].first;
            cost[arrows[i].first][arrows[i].second] =
                labels[i].second;
        }

        for (auto &state : matrix)
            transition_matrix.push_back(
                std::discrete_distribution<>(state.begin(),
                                             state.end()));
    }

    Stat cost_stat;
    for (size_t i = 0; i < ITERATIONS; i++) {
        size_t state = 0;
        real_t proj_cost = 0;
        while (state != N - 1) {
            size_t next = transition_matrix[state](urng);
            proj_cost += cost[state][next];
            state = next;
        }
        cost_stat.save(proj_cost);
    }

    std::ofstream("results.txt")
        << "2025-01-09\nC " << cost_stat.mean();

    return 0;
}
