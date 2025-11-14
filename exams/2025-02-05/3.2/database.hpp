#pragma once

#include <cstddef>
#include <vector>

#include "../../../mocc/server.hpp"
#include "parameters.hpp"

class PostgreSQL
    : public Observer<ItemSupplyRequest>,
      public mocc::Server<SellsUpdateRequest, CacheUpdateResponse> {
  private:
    std::vector<size_t> items_supply;

  public:
    size_t total_oversellings = 0;

    PostgreSQL() : items_supply(k + 1, 0) {}

    void update(ItemSupplyRequest item) override { items_supply[item]++; }

    void update(Host host, SellsUpdateRequest request) override {
        if (items_supply[request.item] < request.item_sales) {
            items_supply[request.item] = 0;
            total_oversellings++;
        } else {
            items_supply[request.item] -= request.item_sales;
        }

        host->update(
            {.item = request.item, .item_supply = items_supply[request.item]}
        );
    }
};
