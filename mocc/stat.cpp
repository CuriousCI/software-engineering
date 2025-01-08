#include "stat.hpp"
#include <cmath>

void Stat::save(real_t data_point) {
    real_t mean_n_1 = mean_ * ((real_t)n / (n + 1)) + data_point / (n + 1);
    m_2__ += (data_point - mean_) * (data_point - mean_n_1);
    mean_ = mean_n_1;
    n++;
}

real_t Stat::mean() const {
    return mean_;
}

real_t Stat::stddev_welford() const {
    return sqrt(n > 0 ? m_2__ / n : 0);
}
