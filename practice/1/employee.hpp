#ifndef EMPLOYEE_HPP_
#define EMPLOYEE_HPP_

#include <random>

#include "../../mocc/stat.hpp"
#include "../../mocc/time.hpp"
#include "parameters.hpp"

class Employee : public Observer<T>,
                 public Observer<ProjInit>,
                 public Notifier<TaskDone, EmplCost> {

    std::vector<std::discrete_distribution<>>
        transition_matrix;
    urng_t &urng;
    size_t phase = 0;
    real_t proj_init = 0;

  public:
    const size_t id;
    const real_t cost;
    Stat comp_time_stat;

    Employee(urng_t &urng, size_t k)
        : urng(urng), id(k),
          cost(1000.0 - 500.0 * (real_t)(k - 1) / (W - 1)) {

        transition_matrix =
            std::vector<std::discrete_distribution<>>(N);

        for (size_t i = 1; i < N; i++) {
            size_t i_0 = i - 1;
            real_t tau = A + B * k * k + C * i * i + D * k * i,
                   alpha = 1 / (F * (G * W - k));

            std::vector<real_t> p(N, 0.0);
            p[i_0] = 1 - 1 / tau;
            p[i_0 + 1] =
                (i_0 == 0 ? (1 - p[i_0])
                          : (1 - alpha) * (1 - p[i_0]));

            for (size_t prev = 0; prev < i_0; prev++)
                p[prev] = alpha * (1 - p[i_0]) / i_0;

            transition_matrix[i_0] =
                std::discrete_distribution<>(p.begin(),
                                             p.end());
        }

        transition_matrix[N - 1] =
            std::discrete_distribution<>{1};
    }

    void update(T t) override {
        if (phase < N - 1) {
            phase = transition_matrix[phase](urng);
            if (phase == N - 1) {
                comp_time_stat.save(t - proj_init);
                notify((real_t)t, cost);
            }
        }
    };

    void update(ProjInit proj_init) override {
        this->proj_init = proj_init;
        phase = 0;
    };
};

#endif
