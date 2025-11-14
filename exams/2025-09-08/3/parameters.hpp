#pragma once

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"
#include <random>

static std::random_device random_device;
static urng_t urng(random_device());

static std::uniform_int_distribution<> random_server;
static std::uniform_int_distribution<> random_product;
static std::uniform_int_distribution<> random_initial_product_amout;
static std::uniform_real_distribution<> random_customer_wait_interval;
static std::uniform_real_distribution<> random_supplier_wait_interval;

static real_t T, H, M, A, B, V, Q;
static size_t C, S, P, F, K;

STRONG_ALIAS(ProductNumber, size_t)
STRONG_ALIAS(CustomerId, size_t)
STRONG_ALIAS(SupplierId, size_t)

struct Demand {
    CustomerId id;
    ProductNumber i;
};

struct Supply {
    SupplierId id;
    ProductNumber i;
};
