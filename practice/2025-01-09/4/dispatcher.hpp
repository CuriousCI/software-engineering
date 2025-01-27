#ifndef DISPATCHER_HPP_
#define DISPATCHER_HPP_

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/observer.hpp"
#include "parameters.hpp"
#include <deque>

class Dispatcher : public Observer<>,
                   public Observer<CustomerId, Request>,
                   public Notifier<CustomerId, RequestCount>,
                   public Notifier<Request> {

    std::deque<Request> requests;

  public:
    std::vector<size_t> requests_count;

    Dispatcher(size_t N) : requests_count(N, 0) {}

    void update() override {
        if (requests.empty())
            return;

        Notifier<Request>::notify(requests.front());
        requests.pop_front();
    }

    void update(CustomerId i, Request r) override {
        requests_count[i - 1]++;
        requests.push_back(r);
        Notifier<CustomerId, RequestCount>::notify(
            i, requests_count[i - 1]);
    }
};

#endif
