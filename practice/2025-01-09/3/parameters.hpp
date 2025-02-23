#pragma once

#include "../../../mocc/mocc.hpp"
#include <cstddef>

static std::random_device random_device;
static urng_t urng(random_device());
static real_t avg, var;
const size_t delta = 1, HORIZON = 1000000;

struct Request {
    real_t t;
};
