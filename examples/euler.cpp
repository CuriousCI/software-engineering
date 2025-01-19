#include <cmath>
#include <fstream>
#include <sstream>

#define SIZE 4

float derivative(float x) { return 2 * x; }

int main() {
    std::ofstream approx[SIZE];

    for (size_t i = 0; i < SIZE; i++) {
        std::stringstream ss;
        ss << "approx-" << i << ".csv";
        approx[i] = std::ofstream(ss.str());
    }

    size_t x[SIZE];

    for (size_t i = 0; i < SIZE; i++) {
        x[i] = 0;
        float delta = 1. / (i + 1);
        for (float t = 0; t <= 10; t += delta) {
            approx[i] << t << ' ' << x[i] << std::endl;
            x[i] += delta * derivative(t);
        }
    }

    for (size_t i = 0; i < SIZE; i++)
        approx[i].close();

    return 0;
}
