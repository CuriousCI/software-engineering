#pragma once

#include "mocc.hpp"
#include <cmath>

/* Dataset which does not store the data points added. */
/* The mean and the standard deviation of the dataset are calculated online. */
class Data {
  private:
    real_t mean_ = 0, m_2__ = 0;
    size_t dataset_size = 0;

  public:
    /* Inserts a data point. */
    /* Updates the mean by using an incremental formula [Sec. 1.3.1]. */
    /* Updates the standard deviation by using Welford's alg. [Sec. 1.3.2]. */
    void insertDataPoint(real_t);

    /* Mean of the data points added. */
    real_t mean() const;
    /* Standard deviation of the data points added. */
    real_t stddev() const;
};
