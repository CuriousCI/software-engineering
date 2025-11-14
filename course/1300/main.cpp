#include <iostream>

#include "../../mocc/mocc.hpp"

struct MarkovDecisionProcess {
    real_t state_vector[2];
};

int main() {
    std::vector<MarkovDecisionProcess>
        markov_decision_processes({{2, 2}, {1, 1}});
    const size_t HORIZON = 10;

    for (size_t time = 0; time <= HORIZON; time++) {
        markov_decision_processes[0].state_vector[0] =
            .7 * markov_decision_processes[0].state_vector[0] +
            .7 * markov_decision_processes[0].state_vector[1];
        markov_decision_processes[0].state_vector[1] =
            -.7 *
                markov_decision_processes[0].state_vector[0] +
            .7 * markov_decision_processes[0].state_vector[1];

        markov_decision_processes[1].state_vector[0] =
            markov_decision_processes[1].state_vector[0] +
            markov_decision_processes[1].state_vector[1];
        markov_decision_processes[1].state_vector[1] =
            -markov_decision_processes[1].state_vector[0] +
            markov_decision_processes[1].state_vector[1];

        std::cout << time << ' ';
        for (auto markov_decision_process :
             markov_decision_processes)
            for (auto component :
                 markov_decision_process.state_vector) {
                std::cout << component << ' ';
            }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
