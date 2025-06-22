#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/observer.hpp"
#include "parameters.hpp"
#include <deque>

class Dispatcher : public Observer<>,
                   public Observer<CustomerId, PurchaseRequest>,
                   public Notifier<CustomerId, RequestsCount>,
                   public Notifier<PurchaseRequest> {

    std::deque<PurchaseRequest> requests;

  public:
    std::vector<size_t> requests_count;

    Dispatcher(size_t N) : requests_count(N, 0) {}

    void update() override {
        if (requests.empty())
            return;

        Notifier<PurchaseRequest>::notify(requests.front());
        requests.pop_front();
    }

    void update(CustomerId id, PurchaseRequest request) override {
        requests_count[id - 1]++;
        requests.push_back(request);
        Notifier<CustomerId, RequestsCount>::notify(id, requests_count[id - 1]);
    }
};
