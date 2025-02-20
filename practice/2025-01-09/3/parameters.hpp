#pragma once

#include "../../../mocc/mocc.hpp"
#include <cstddef>

static std::random_device random_device;
static urng_t urng(random_device());
const size_t T = 1, HORIZON = 1000000;

struct Request {
    real_t t;
};
