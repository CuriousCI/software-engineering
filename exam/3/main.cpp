#include <fstream>
#include <ostream>
#include <random>

#include "../case/case.hpp"
#include "../case/time.hpp"
#include "database.hpp"
#include "dispatcher.hpp"
#include "env.hpp"
#include "monitor.hpp"
#include "server.hpp"

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    Timer timer;
    Env env(random_engine);
    Dispatcher dispatcher(random_engine);
    PostgreSQL dbms;
    std::vector<Server *> servers;
    std::vector<Monitor *> monitors;

    for (size_t id = 1; id <= Q; id++) {
        auto server = new Server(random_engine, rho, id);
        auto monitor = new Monitor(id);

        servers.push_back(server);
        monitors.push_back(monitor);

        timer.addSubscriber(server);
        timer.addSubscriber(monitor);
        dispatcher.Publisher<HttpRequest>::addSubscriber(server);
        server->Publisher<HttpResponse>::addSubscriber(&dispatcher);
        server->Publisher<HttpResponse>::addSubscriber(monitor);
        servers.back()->lib::Client<Query, Reply>::addSubscriber(&dbms);
    }

    timer.addSubscriber(&env);
    env.addSubscriber(&dispatcher);

    while (timer.time < HORIZON)
        timer.tick();

    std::ofstream("outputs.txt")
        << "Avg StdDev (ID = " << ID << ", "
        << "MyMagicNumber = " << MY_MAGIC_NUMBER << ", "
        << "time = " << time(NULL) << ")" << std::endl;

    for (auto m : monitors)
        std::ofstream("outputs.txt", std::ios_base::app)
            << m->id << ' '
            << m->response_time.mean() << ' '
            << m->response_time.stddev() << std::endl;
}
