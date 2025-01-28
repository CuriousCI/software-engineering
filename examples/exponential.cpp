#include <iostream>
#include <random>

using urng_t = std::default_random_engine;
using real_t = float;
const size_t HORIZON = 10000;

int main() {
    std::random_device random_device;
    urng_t urng(random_device());
    std::exponential_distribution<> random_interval(5. / 60.);

    real_t next_request_time = 0;
    std::vector<real_t> req_per_min = {0};
    for (real_t time_s = 0; time_s < HORIZON; time_s++) {
        if (((size_t)time_s) % 60 == 0)
            req_per_min.push_back(0);

        if (time_s < next_request_time)
            continue;

        req_per_min.back()++;
        next_request_time = time_s + random_interval(urng);
    }

    real_t mean = 0;
    for (auto x : req_per_min)
        mean += x;

    std::cout << mean / req_per_min.size() << std::endl;
}
