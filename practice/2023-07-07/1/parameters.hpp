#pragma once

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"

static std::random_device random_device;
static urng_t urng(random_device());
static real_t A, B, C, D, F, G;
static size_t N, W, HORIZON = 100000;

STRONG_ALIAS(ProjInitTime, real_t)
STRONG_ALIAS(TaskDoneTime, real_t)
STRONG_ALIAS(EmployeeCost, real_t)
