#include <iostream>

#include "../software.hpp"

struct MarkovDecisionProcess {
    real_t state_vector[2];
};

int main() {
    std::uniform_real_distribution<> random_uniform(0, 1);
    std::vector<MarkovDecisionProcess>
        markov_decision_processes(2, {0, 0});

    const size_t HORIZON = 10;

    for (size_t time = 0; time <= HORIZON; time++) {
        for (size_t component = 0; component < 2;
             component++) {
            markov_decision_processes[0]
                .state_vector[component] =
                markov_decision_processes[1]
                    .state_vector[component] *
                random_uniform(urng);
            markov_decision_processes[1]
                .state_vector[component] =
                markov_decision_processes[0]
                    .state_vector[component] +
                random_uniform(urng);
        }

        std::cout << time << ' ';
        for (auto markov_decision_process :
             markov_decision_processes) {
            for (auto component :
                 markov_decision_process.state_vector) {
                std::cout << component << ' ';
            }
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
