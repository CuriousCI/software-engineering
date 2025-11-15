#include <fstream>
#include <iostream>
#include <random>

#include "../../../mocc/math.hpp"
#include "../../../mocc/mocc.hpp"
#include <random>

int main() {
    size_t N;
    std::vector<std::discrete_distribution<>> transition_matrix;
    matrix transition_cost;

    {
        std::ifstream parameters("parameters.txt");

        struct Transition {
            size_t i, j;
            real_t probability, cost;
        };

        char format;
        std::vector<Transition> transitions;

        while (parameters >> format)
            switch (format) {
            case 'N':
                parameters >> N;
                break;
            case 'A':
                Transition transition;
                parameters >> transition.i >> transition.j >>
                    transition.probability >> transition.cost;
                transitions.push_back(transition);
                break;
            }

        parameters.close();

        transition_cost = matrix(N, std::vector<real_t>(N, 0));
        matrix transition_probability(N, std::vector<real_t>(N, 0));
        transition_probability[N - 1][N - 1] = 1;

        for (auto &transition : transitions) {
            transition_probability[transition.i][transition.j] =
                transition.probability;
            transition_cost[transition.i][transition.j] = transition.cost;
        }

        for (auto &row : transition_probability) {
            transition_matrix.push_back(
                std::discrete_distribution<>(row.begin(), row.end())
            );
        }
    }

    OnlineDataAnalysis project_cost_analysis;

    urng_t urng = pseudo_random_engine_from_device();
    const size_t ITERATIONS = 1000;
    const size_t INITIAL_STATE = 0;

    for (size_t _ = 0; _ < ITERATIONS; _++) {
        size_t current_state = INITIAL_STATE;
        real_t project_cost = 0;

        while (current_state < N - 1) {
            size_t next_state = transition_matrix[current_state](urng);
            project_cost += transition_cost[current_state][next_state];
            current_state = next_state;
        }

        project_cost_analysis.insertDataPoint(project_cost);
    }

    std::ofstream("results.txt")
        << "2025-01-09\nC " << project_cost_analysis.mean() << std::endl;

    return 0;
}
