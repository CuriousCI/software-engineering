#include <cmath>
#include <fstream>
#include <sstream>

/* f(x) = x^2; */
/* f'(x) = 2x */
float f_prime(float x) { return 2 * x; }

int main() {
    const size_t FILES_COUNT = 4;
    std::ofstream approximation_files[FILES_COUNT];

    for (size_t file = 0; file < FILES_COUNT; file++) {
        std::stringstream ss;
        ss << file << ".csv";
        approximation_files[file] = std::ofstream(ss.str());
    }

    float y[FILES_COUNT];

    for (size_t file = 0; file < FILES_COUNT; file++) {
        y[file] = 0;
        /* The bigger the file index, the smaller the Delta. */
        const float delta = 1.0 / (file + 1);
        for (float x = 0; x <= 10; x += delta) {
            approximation_files[file] << x << ' ' << y[file]
                                      << std::endl;
            y[file] += delta * f_prime(x);
        }
    }

    for (size_t file = 0; file < FILES_COUNT; file++) {
        approximation_files[file].close();
    }

    return EXIT_SUCCESS;
}
