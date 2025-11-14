#pragma once

#include "../../../mocc/mocc.hpp"
#include <random>

static std::random_device random_device;
static urng_t urng(random_device());
static std::uniform_real_distribution<real_t> random_position;
static real_t T, H, M, N, L, V, A, D;
