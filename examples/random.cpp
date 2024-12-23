#include <iostream>
#include <random>

int main() {
    std::cout << random() << std::endl;

    std::random_device random_device;
    std::cout << random_device() << std::endl;

    std::default_random_engine random_engine(random_device());
    std::cout << random_engine() << std::endl;

    return 0;
}
