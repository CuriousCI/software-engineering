#include "team.hpp"

Team::Team(std::default_random_engine &random_engine, size_t k)
    : random_engine(random_engine),
      id(k),
      cost(1000.0 - 500.0 * (real_t)(k - 1) / (W - 1)) {

    transition_matrix = std::vector<std::discrete_distribution<>>(N);

    for (size_t i = 1; i < N; i++) {
        size_t i_0 = i - 1;
        real_t theta = A + B * k * k + C * i * i + D * i * k,
               alpha = 1.0 / (F * (G * W - k)),
               tau = T * theta;

        std::vector<real_t> p(N, 0.0);
        p[i_0] = 1.0 - 1.0 / tau;
        p[i_0 + 1] = (i_0 == 0 ? (1.0 - p[i_0]) : (1.0 - alpha) * (1.0 - p[i_0]));
        for (size_t prev = 0; prev < i_0; prev++)
            p[prev] = alpha * (1.0 - p[i_0]) / i_0;

        transition_matrix[i_0] = std::discrete_distribution<>(p.begin(), p.end());
    }

    transition_matrix[N - 1] = std::discrete_distribution<>{1};
}

void Team::update(ProjInitTime proj_init_time) {
    this->proj_init_time = proj_init_time;
    phase = 0;
}

void Team::update(Time time) {
    if (phase < N - 1) {
        phase = transition_matrix[phase](random_engine);
        if (phase == N - 1) {
            completion_time.save(time - proj_init_time);
            notifySubscribers(cost, (real_t)time);
        }
    }
}
