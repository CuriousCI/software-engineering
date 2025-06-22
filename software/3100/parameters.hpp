#pragma once

#include "../../mocc/mocc.hpp"
#include <cstddef>
#include <random>

enum Light { GREEN = 0, YELLOW = 1, RED = 2 };
static std::random_device random_device;
static urng_t urng = urng_t(random_device());
const size_t HORIZON = 1000;
