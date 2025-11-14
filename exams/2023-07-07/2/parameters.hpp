#pragma once

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"
#include <cstddef>

struct Task {
    size_t v, k;
};

STRONG_ALIAS(Job, size_t);
STRONG_ALIAS(TaskDone, Task);

static std::random_device random_device;
static urng_t urng(random_device());
static size_t W, N, B, L, D, HORIZON = 100000;
