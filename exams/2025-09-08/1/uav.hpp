#pragma once

#include "../../../mocc/mocc.hpp"
#include "../../../mocc/system.hpp"
#include "parameters.hpp"
#include <cmath>
#include <random>
#include <vector>

class UAV : public SystemObserver {

  public:
    std::vector<real_t> x, v, p;

    UAV() : x(3, 0), v(3, 0), p(3, 0) {
        for (size_t k = 0; k < 3; k++) {
            x[k] = random_position(urng);
        }
    }

    void update() override {
        for (size_t k = 0; k < 3; k++) {
            x[k] += v[k] * T;
            p[k] = exp(-A * ((x[k] + L) / (2 * L)));
            std::bernoulli_distribution speed(p[k]);
            v[k] = speed(urng) ? V : -V;
        }
    }
};
