#include <cmath>
#include <fstream>
#include <sstream>

/* y(x) = x^2 => y'(x) = 2x */
float y_prime(float x) { return 2 * x; }

int main() {
    const size_t DELTA_COUNT = 4;
    std::ofstream results_files[DELTA_COUNT];

    for (size_t index = 0; index < DELTA_COUNT; index++) {
        std::stringstream ss;
        ss << "appr-" << index << ".csv";
        results_files[index] = std::ofstream(ss.str());
    }

    for (size_t index = 0; index < DELTA_COUNT; index++) {
        float y = 0.0;
        const float delta = 1.0 / (float)(index + 1);
        for (float x = 0; x <= 10; x += delta) {
            results_files[index] << x << ' ' << y << std::endl;
            y += delta * y_prime(x);
        }
    }

    for (size_t index = 0; index < DELTA_COUNT; index++) {
        results_files[index].close();
    }

    return EXIT_SUCCESS;
}
