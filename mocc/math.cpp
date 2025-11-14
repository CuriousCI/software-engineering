#include "math.hpp"

// clang-format off

void OnlineDataAnalysis::insertDataPoint(real_t data_point) {
    real_t mean_prev_size =
        mean_ * (
            (real_t)number_of_data_points / 
            (number_of_data_points + 1)
        ) +
        data_point / (number_of_data_points + 1);

    m_2__ += 
        (data_point - mean_) * (data_point - mean_prev_size);
    mean_ = mean_prev_size;
    number_of_data_points++;
}

real_t OnlineDataAnalysis::mean() const { return mean_; }

real_t OnlineDataAnalysis::stddev() const {
    return number_of_data_points > 0 ? 
        sqrt(m_2__ / number_of_data_points) : 0;
}

// clang-format on
