#ifndef CUSTOMER_HPP_
#define CUSTOMER_HPP_

#include "../../../mocc/notifier.hpp"
#include "../../../mocc/time.hpp"
#include "parameters.hpp"
#include <random>

class Customer : public Observer<class T>,
                 public Notifier<Request> {
    urng_t &urng;
    real_t next_request = 0;
    std::normal_distribution<> random_interval;

  public:
    Customer(urng_t &urng, real_t avg, real_t var)
        : urng(urng), random_interval(avg, var) {}

    void update(class T time) override {
        if (time < next_request)
            // IDLE
            return;

        // REQUEST
        notify(Request{time});
        real_t interval = random_interval(urng);
        next_request = time + interval;
    }
};

#endif
