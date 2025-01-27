#ifndef CUSTOMER_HPP_
#define CUSTOMER_HPP_

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public Observer<class T>,
                 public Notifier<CustomerId, Request> {
    urng_t &urng;
    real_t next_request = 0;
    std::normal_distribution<> random_interval;

  public:
    const size_t i;

    Customer(urng_t &urng, real_t avg, real_t var, size_t i)
        : urng(urng), random_interval(avg, var), i(i) {}

    void update(class T t) override {
        if (t < next_request)
            // IDLE
            return;

        // REQUEST
        notify(i, Request{t});
        real_t interval = random_interval(urng);
        next_request = t + interval;
    }
};

#endif
