#pragma once

#include <random>

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/system.hpp"
#include "parameters.hpp"

class Customer : public SystemObserver, public Notifier<ItemPurchaseRequest> {
  public:
    void update() override {
        size_t item = items_purchase_distribution(urng);
        if (item > 0) {
            notify(item);
        }
    }
};
