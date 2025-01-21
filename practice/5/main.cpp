#include <fstream>
#include <ostream>
#include <random>

#include "../case/case.hpp"
#include "../case/time.hpp"
#include "controller.hpp"
#include "data.hpp"
#include "disturbance.hpp"
#include "monitor.hpp"
#include "plant.hpp"
#include "user.hpp"

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    Timer timer;
    User user;
    Plant plant;
    Monitor monitor;
    Controller controller;
    Disturbance disturbance(random_engine);

    timer.addSubscriber(&disturbance);
    timer.addSubscriber(&plant);
    user.addSubscriber(&controller);
    user.addSubscriber(&monitor);
    plant.addSubscriber(&controller);
    plant.addSubscriber(&monitor);
    controller.addSubscriber(&plant);
    disturbance.addSubscriber(&plant);

    user.notifySubscribers(350); // r = 350 | r = 280

    while (timer.time < HORIZON)
        timer.tick();

    std::ofstream("outputs.txt")
        << "Avg StdDev ("
        << "ID = " << ID << ", "
        << "MyMagicNumber = " << MY_MAGIC_NUMBER << ", "
        << "time = " << time(NULL) << ")" << std::endl
        << monitor.err.mean() << ' '
        << monitor.err.stddev() << std::endl;
}
