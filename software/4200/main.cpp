#include <cstddef>
#include <random>

using real_t = double;

class Task {
    std::default_random_engine &random_engine;
    std::vector<std::discrete_distribution<>>
        transition_matrix;

  public:
    size_t phase = 0, costs = 0;
    Task(std::default_random_engine &random_engine)
        : random_engine(random_engine) {
        transition_matrix = {
            {0, 1},          {0, .3, .7},
            {0, 0, .2, .8},  {0, .1, .1, .1, .7},
            {0, 0, 0, 0, 1},
        };
    }

    void next() {
        if (phase == 1 || phase == 3)
            costs += 20;
        else if (phase == 2)
            costs += 40;

        phase = transition_matrix[phase](random_engine);
    }
};

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    Task task(random_engine);
    real_t average = 0, samples = 0, time = 0, step = 1;

    // avg = avg * (((double)i) / ((double)(i + 1))) +
    //       y[0] / ((double)(i + 1));

    // simulator output time and cost

    while (task.phase != 4) {
        time += step;
        task.next();
    }

    return 0;
}
