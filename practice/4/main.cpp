#include <fstream>
#include <ostream>
#include <random>

#include "../case/case.hpp"
#include "../case/util.hpp"
#include "database.hpp"
#include "dispatcher.hpp"
#include "env.hpp"
#include "monitor.hpp"
#include "server.hpp"

int main() {
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());

    real_t tau = 2, rho = 20;
    size_t N = 300, F = 10, K = 20, S = 10, Q = 5, HORIZON = 1000000;

    Clock clock;
    Env env(random_engine, tau, N, F);
    Dispatcher dispatcher(random_engine);
    PostgreSQL dbms(F, K);
    std::vector<Server *> servers;
    std::vector<Monitor *> monitors;

    {
        for (size_t id = 1; id <= Q; id++) {
            auto server = new Server(random_engine, rho, id);
            auto monitor = new Monitor(id);

            servers.push_back(server);
            monitors.push_back(monitor);

            clock.addSubscriber(server);
            clock.addSubscriber(monitor);
            dispatcher.Publisher<HttpRequest>::addSubscriber(server);
            server->Publisher<HttpResponse>::addSubscriber(&dispatcher);
            server->Publisher<HttpResponse>::addSubscriber(monitor);
            servers.back()->lib::Client<Query, Reply>::addSubscriber(&dbms);
        }

        clock.addSubscriber(&env);
        env.addSubscriber(&dispatcher);
    }

    while (clock.time < HORIZON)
        clock.tick();

    {
        std::ofstream output("outputs.txt");

        output << " OutputIndex Load (ID = " << ID
               << ", MyMagicNumber = " << MY_MAGIC_NUMBER
               << ", time = " << time(NULL)
               << ")" << std::endl;

        for (auto m : monitors)
            output << m->id << ' '
                   << m->response_time.mean() << ' '
                   << m->response_time.stddev() << std::endl;

        output.close();
    }
}
