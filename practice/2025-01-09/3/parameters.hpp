#pragma once

#include "../../../mocc/mocc.hpp"
#include <cstddef>

static std::random_device random_device;
static urng_t urng(random_device());
static real_t AVG, VAR;
const size_t T = 1, HORIZON = 1000000;

struct PurchaseRequest {
    real_t t;
};
