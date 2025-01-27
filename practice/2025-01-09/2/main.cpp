#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/mocc.hpp"
#include <random>

int main() {
    std::ifstream parameters("parameters.txt");
    std::random_device random_device;
    urng_t urng(random_device());

    size_t N, C;
    const size_t ITERATIONS = 1000;

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
            case 'C':
                parameters >> C;
                break;
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

    size_t tries = 0, succ = 0;

    for (size_t s = 0; s < ITERATIONS; s++) {
        size_t state = 0;
        real_t proj_cost = 0;
        tries++;

        while (state != N - 1) {
            size_t next = transition_matrix[state](urng);
            proj_cost += cost[state][next];
            state = next;
        }

        if (proj_cost <= C)
            succ++;
    }

    std::ofstream("results.txt")
        << "2025-01-09\nP " << (real_t)succ / (real_t)tries;

    return 0;
}
