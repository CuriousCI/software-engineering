#include "server.hpp"

Server::Server(std::default_random_engine &random_engine, const real_t rho, const size_t id) :
    random_engine(random_engine),
    random_response_time(rho),
    id(id) {}

void Server::update(HttpRequest http_request) {
    if (http_requests.empty())
        next_query_time = ((Data)http_request).time + random_response_time(random_engine);

    http_requests.push_back(http_request);
}

void Server::update(Reply reply) {
    Publisher<HttpResponse>::notifySubscribers((Resp)reply);
}

void Server::update(Time time) {
    if (http_requests.empty())
        return;

    if (time < next_query_time)
        return;

    Publisher<Host, Query>::notifySubscribers(this, (Data)http_requests.front());
    if (!http_requests.empty())
        next_query_time = time + random_response_time(random_engine);

    http_requests.pop_front();
}
