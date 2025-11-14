#include <iostream>

#include "listings.hpp"

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::discrete_distribution<>
        markov_chain_transition_matrix[] = {
            {0, 1},
            {0, 0.3, 0.7},
            {0, 0.2, 0.2, 0.6},
            {0, 0.1, 0.2, 0.1, 0.6},
            {1},
        };

    const size_t HORIZON = 15, FIRST_STATE = 0;

    size_t current_state = FIRST_STATE;
    for (size_t _ = 0; _ < HORIZON; _++) {
        std::cout << current_state << std::endl;

        size_t next_state =
            markov_chain_transition_matrix[current_state](
                pseudo_random_engine
            );

        current_state = next_state;
    }

    return EXIT_SUCCESS;
}
