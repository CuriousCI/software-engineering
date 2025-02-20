#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include "../../mocc/alias.hpp"
#include "../../mocc/mocc.hpp"

STRONG_ALIAS(ProjInit, real_t)
STRONG_ALIAS(TaskDone, real_t)
STRONG_ALIAS(EmplCost, real_t)

static real_t A, B, C, D, F, G;
static size_t N, W, HORIZON = 100000;

#endif
