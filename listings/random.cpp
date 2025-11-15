#include <iostream>
#include <random>

int main() {
    std::cout << random() << std::endl;

    std::random_device device_randomness_source; /* 1 */
    std::cout << device_randomness_source() << std::endl;

    auto seed = device_randomness_source(); /* 2 */

    std::default_random_engine /* 3 */
        pseudo_random_engine(seed);
    std::cout << pseudo_random_engine() /* 4 */ << std::endl;

    return EXIT_SUCCESS;
}
