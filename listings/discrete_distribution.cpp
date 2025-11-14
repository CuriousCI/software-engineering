#include <iostream>

#include "listings.hpp"

enum Item { Shirt = 0, Hoodie = 1, Pants = 2 };

int main() {
    std::default_random_engine pseudo_random_engine =
        pseudo_random_engine_from_device();

    std::discrete_distribution<> random_item = {232, 158, 288};

    for (size_t request = 0; request < 1000; request++) {
        switch (random_item(pseudo_random_engine)) {
        case Shirt:
            std::cout << "shirt";
            break;
        case Hoodie:
            std::cout << "hoodie";
            break;
        case Pants:
            std::cout << "pants";
            break;
        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
