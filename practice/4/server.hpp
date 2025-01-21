#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <deque>
#include <random>

#include "../case/serv.hpp"
#include "../case/util.hpp"
#include "data.hpp"

class Server :
    public Subscriber<Time>,
    public Subscriber<HttpRequest>,
    public Publisher<HttpResponse>,
    public lib::Client<Query, Reply>
{
    std::default_random_engine &random_engine;
    std::poisson_distribution<size_t> random_response_time;
    std::deque<HttpRequest> http_requests;
    real_t next_query_time = 0;

   public:
    const size_t id;

    Server(std::default_random_engine &, const real_t rho, const size_t id);

    void update(Time) override;
    void update(HttpRequest) override;
    void update(Reply) override;
};

#endif
