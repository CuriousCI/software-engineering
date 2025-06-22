#include "math.hpp"

void Data::insertDataPoint(real_t data_point) {
    real_t mean_prev_size =
        mean_ * ((real_t)dataset_size / (dataset_size + 1)) +
        data_point / (dataset_size + 1);

    m_2__ += (data_point - mean_) * (data_point - mean_prev_size);
    mean_ = mean_prev_size;
    dataset_size++;
}

real_t Data::mean() const { return mean_; }

real_t Data::stddev() const {
    return dataset_size > 0 ? sqrt(m_2__ / dataset_size) : 0;
}
