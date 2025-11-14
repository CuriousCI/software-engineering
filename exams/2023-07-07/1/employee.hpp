#pragma once

#include <random>

#include "../../../mocc/math.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Employee : public Observer<StopwatchElapsedTime>,
                 public Observer<ProjInitTime>,
                 public Notifier<TaskDoneTime, EmployeeCost> {

    std::vector<std::discrete_distribution<>> transition_matrix;
    size_t phase = 0;
    real_t project_init_time = 0;

  public:
    const size_t id;
    const real_t cost;
    OnlineDataAnalysis completion_time_analysis;

    Employee(urng_t &urng, size_t k)
        : id(k), cost(1000.0 - 500.0 * (real_t)(k - 1) / (W - 1)) {

        transition_matrix = std::vector<std::discrete_distribution<>>(N);

        for (size_t i = 1; i < N; i++) {
            size_t i_0 = i - 1;
            real_t tau = A + B * k * k + C * i * i + D * k * i,
                   alpha = 1 / (F * (G * W - k));

            std::vector<real_t> p(N, 0.0);
            p[i_0] = 1 - 1 / tau;
            p[i_0 + 1] = (i_0 == 0 ? (1 - p[i_0]) : (1 - alpha) * (1 - p[i_0]));

            for (size_t prev = 0; prev < i_0; prev++)
                p[prev] = alpha * (1 - p[i_0]) / i_0;

            transition_matrix[i_0] =
                std::discrete_distribution<>(p.begin(), p.end());
        }

        transition_matrix[N - 1] = std::discrete_distribution<>{1};
    }

    void update(StopwatchElapsedTime elapsed_time) override {
        if (phase < N - 1) {
            phase = transition_matrix[phase](urng);
            if (phase == N - 1) {
                completion_time_analysis.insertDataPoint(
                    elapsed_time - project_init_time
                );
                notify((real_t)elapsed_time, cost);
            }
        }
    };

    void update(ProjInitTime project_init_time) override {
        this->project_init_time = project_init_time;
        phase = 0;
    };
};
