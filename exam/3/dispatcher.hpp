#ifndef DISPATCHER_HPP_
#define DISPATCHER_HPP_

#include <random>

#include "../case/publisher.hpp"
#include "../case/subscriber.hpp"
#include "data.hpp"

class Dispatcher :
    public Subscriber<HttpRequest>,
    public Subscriber<HttpResponse>,
    public Publisher<HttpRequest>,
    public Publisher<HttpResponse>
{
    std::default_random_engine &random_engine;
    std::uniform_int_distribution<size_t> random_server;

   public:
    Dispatcher(std::default_random_engine &);

    void notifySubscribers(HttpRequest) override;
    void update(HttpRequest) override;
    void update(HttpResponse) override;
};

#endif
