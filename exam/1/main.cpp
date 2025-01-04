#include <ctime>
#include <fstream>
#include <ostream>

#include "../case/case.hpp"
#include "../case/time.hpp"
#include "data.hpp"
#include "project.hpp"
#include "team.hpp"

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    Timer timer(T);
    Project project;
    std::vector<Team *> teams;

    for (size_t k = 1; k <= W; k++) {
        Team *team = new Team(random_engine, k);

        project.addSubscriber(team);
        teams.push_back(team);
        timer.addSubscriber(team);
        team->addSubscriber(&project);
    }

    while (timer.time < HORIZON) {
        bool stop = timer.time > 1000;

        for (auto t : teams)
            if (t->completion_time.stddev() > 0.01 * t->completion_time.mean()) {
                stop = false;
                break;
            }

        if (stop)
            break;

        timer.tick();
    }

    std::ofstream("outputs.txt")
        << "Dipendente AvgTime AvgCost StdDevTime StdDevCost ("
        << "ID = " << ID << ", "
        << "MyMagicNumber = " << MY_MAGIC_NUMBER << ", "
        << "time = " << time(NULL) << ")" << std::endl
        << "A = " << A << ", "
        << "B = " << B << ", "
        << "C = " << C << ", "
        << "D = " << D << ", "
        << "F = " << F << ", "
        << "G = " << G << ", "
        << "N = " << N << ", "
        << "W = " << W << ", "
        << "AvgTime = " << project.time.mean() << ", "
        << "AvgCosto = " << project.cost.mean() << std::endl;

    for (auto t : teams)
        std::ofstream("outputs.txt", std::ios_base::app)
            << t->id << ' '
            << t->completion_time.mean() << ' '
            << t->completion_time.mean() * t->cost << ' '
            << t->completion_time.stddev() << ' '
            << t->completion_time.stddev() * t->cost << std::endl;

    return 0;
}
