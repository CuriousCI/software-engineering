#include <iostream>
#include <random>

int main() {
    std::cout << random() << std::endl;

    std::random_device device_randomness_source;
    std::cout << device_randomness_source() << std::endl;

    std::default_random_engine pseudo_random_engine(
        device_randomness_source()
    );
    std::cout << pseudo_random_engine() << std::endl;

    return EXIT_SUCCESS;
}
