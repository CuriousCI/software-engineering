#ifndef DATA_HPP_
#define DATA_HPP_

#include "../case/alias.hpp"
#include "../case/case.hpp"

ALIAS_TYPE(ProjInitTime, real_t)
ALIAS_TYPE(TaskCompTime, real_t)
ALIAS_TYPE(TeamCost, real_t)

const real_t A = 1, B = 1, C = 1, D = 1, F = 1, G = 2, T = 1, HORIZON = 10000000;
const size_t N = 5, W = 3;

#endif
