#include <fstream>
#include <ostream>
#include <random>

#include "../case/case.hpp"
#include "../case/time.hpp"
#include "env.hpp"
#include "monitor.hpp"

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    Timer timer;
    Env env(random_engine);
    Monitor monitor;

    timer.addSubscriber(&env);
    env.addSubscriber(&monitor);

    while (timer.time < HORIZON)
        timer.tick();

    std::ofstream("outputs.txt")
        << "Avg StdDev (ID = " << ID << ", "
        << "MyMagicNumber = " << MY_MAGIC_NUMBER << ", "
        << "time = " << time(NULL) << ")" << std::endl
        << monitor.interval.mean() << ' '
        << monitor.interval.stddev() << std::endl;
}
