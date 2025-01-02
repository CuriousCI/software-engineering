#include <iostream>
#include <random>

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_int_distribution<> random_T(20, 30);

    size_t T = random_T(random_engine), next_request_time = T;
    for (size_t time = 0; time < 1000; time++) {
        if (time < next_request_time)
            continue;

        std::cout << T << std::endl;
        T = random_T(random_engine);
        next_request_time = time + T;
    }

    return 0;
}
