#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include "../../mocc/alias.hpp"
#include <cstddef>

struct Task {
    size_t v, k;
};

STRONG_ALIAS(Job, size_t);
STRONG_ALIAS(TaskDone, Task);

static size_t W, N, B, L, D, HORIZON = 100000;

#endif
