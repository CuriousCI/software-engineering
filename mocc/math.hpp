#ifndef MOCC_STAT_HPP_
#define MOCC_STAT_HPP_

#include "mocc.hpp"

class Stat {
    real_t mean_ = 0, m_2__ = 0;
    size_t n = 0;

  public:
    void save(real_t);

    real_t mean() const;
    real_t stddev() const;
};

#endif
