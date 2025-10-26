#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/system.hpp"
#include "parameters.hpp"

class Supplier : public SystemObserver, public Notifier<ItemSupplyRequest> {
  public:
    void update() override {
        size_t item = items_supply_distribution(urng);
        if (item > 0) {
            notify(item);
        }
    }
};
