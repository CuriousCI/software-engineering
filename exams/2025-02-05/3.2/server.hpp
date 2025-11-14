#pragma once

#include "../../../mocc/buffer.hpp"
#include "../../../mocc/notifier.hpp"
#include "../../../mocc/server.hpp"
#include "../../../mocc/system.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"

class Server : public SystemObserver,
               public TimerBasedEntity,
               public Buffer<ItemPurchaseRequest>,
               public Notifier<AvailabilityResponse>,
               public mocc::Client<SellsUpdateRequest, CacheUpdateResponse> {
  private:
    std::vector<size_t> items_supply_cache;
    std::vector<size_t> items_sales;

    bool is_updating = false;

  public:
    Server(System &system)
        : Buffer(1000), TimerBasedEntity(system, 0, TimerMode::Once),
          items_supply_cache(k + 1, 0), items_sales(k + 1, 0) {
        system.addObserver(this);
    }

    void update() override {
        if (is_updating)
            return;

        if (!buffer.empty()) {
            auto item = buffer.front();
            buffer.pop_front();

            if (items_sales[item] < items_supply_cache[item]) {
                items_sales[item]++;
                Notifier<AvailabilityResponse>::notify(true);
            } else {
                Notifier<AvailabilityResponse>::notify(false);
            }
        }

        if (postgresql_update(urng)) {
            is_updating = true;
            this->timer.resetWithDuration(10);
            for (size_t item = 1; item <= k; item++) {
                Notifier<Host, SellsUpdateRequest>::notify(
                    this, {.item = item, .item_sales = items_sales[item]});
                items_sales[item] = 0;
            }
        }
    }

    void update(CacheUpdateResponse response) override {
        this->items_supply_cache[response.item] = response.item_supply;
    }

    void update(TimerEnded) override { is_updating = false; }
};
