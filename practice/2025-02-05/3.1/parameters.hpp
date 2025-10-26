#pragma once

#include <cstddef>
#include <random>

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"

static std::random_device random_device;
static urng_t urng(random_device());
const size_t T = 1, H = 10000;
static real_t T1, T2;

// static std::discrete_distribution<> items_purchase_distribution;
// static std::discrete_distribution<> items_supply_distribution;
// static std::bernoulli_distribution postgresql_update;
// static size_t n, k, H;
// static real_t alpha;
// struct SellsUpdateRequest {
//     size_t item, item_sales;
// };
//
// struct CacheUpdateResponse {
//     size_t item, item_supply;
// };
//
// STRONG_ALIAS(ItemPurchaseRequest, size_t)
// STRONG_ALIAS(ItemSupplyRequest, size_t)
// STRONG_ALIAS(AvailabilityResponse, bool)
