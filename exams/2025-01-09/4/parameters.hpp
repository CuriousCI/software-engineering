#pragma once

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"
#include <cstddef>

static std::random_device random_device;
static urng_t urng(random_device());
static real_t AVG, VAR;
static size_t N;
const size_t T = 1, HORIZON = 1000000;

struct CustomerPurchaseRequest {
    real_t time;
};

STRONG_ALIAS(CustomerId, size_t);
STRONG_ALIAS(RequestsCount, size_t);
