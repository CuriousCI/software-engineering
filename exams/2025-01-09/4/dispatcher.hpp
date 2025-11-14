#pragma once

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/observer.hpp"
#include "parameters.hpp"
#include <deque>

using NotifierCustomerRequests = Notifier<CustomerId, RequestsCount>;

class Dispatcher : public Observer<>,
                   public Observer<CustomerId, CustomerPurchaseRequest>,
                   public NotifierCustomerRequests,
                   public Notifier<CustomerPurchaseRequest> {

    std::deque<CustomerPurchaseRequest> requests;

  public:
    std::vector<size_t> requests_count;

    Dispatcher(size_t N) : requests_count(N, 0) {}

    void update() override {
        if (requests.empty())
            return;

        Notifier<CustomerPurchaseRequest>::notify(requests.front());
        requests.pop_front();
    }

    void
    update(CustomerId customer_id, CustomerPurchaseRequest request) override {
        requests_count[customer_id - 1]++;
        requests.push_back(request);
        NotifierCustomerRequests::notify(
            customer_id, requests_count[customer_id - 1]
        );
    }
};
