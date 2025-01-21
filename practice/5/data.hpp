#ifndef DATA_HPP_
#define DATA_HPP_

#include "../case/alias.hpp"
#include "../case/case.hpp"

ALIAS_TYPE(TempSetPoint, real_t)
ALIAS_TYPE(Dist, real_t)
ALIAS_TYPE(Ctrl, real_t);
ALIAS_TYPE(Temp, real_t);

const real_t Td = 1, T = 0.001, p = -1, k1 = -(p * p), k2 = 2 * p, a = 10, b = 1,
             HORIZON = 1000000;

#endif
