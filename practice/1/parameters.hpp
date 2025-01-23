#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include "../../mocc/alias.hpp"
#include "../../mocc/mocc.hpp"

ALIAS_TYPE(ProjInit, real_t)
ALIAS_TYPE(TaskDone, real_t)
ALIAS_TYPE(EmplCost, real_t)

static real_t A, B, C, D, F, G;
static size_t N, W, HORIZON = 100000;

#endif
