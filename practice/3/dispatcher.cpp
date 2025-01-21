#include "dispatcher.hpp"

Dispatcher::Dispatcher(std::default_random_engine &random_engine) :
    random_engine(random_engine) {}

void Dispatcher::notifySubscribers(HttpRequest http_request) {
    auto servers = Publisher<HttpRequest>::subscribers;

    if (servers.empty())
        return;

    random_server.param(std::uniform_int_distribution<size_t>::param_type{0, servers.size() - 1});
    servers[random_server(random_engine)]->update(http_request);
}

void Dispatcher::update(HttpRequest http_request) {
    notifySubscribers(http_request);
}

void Dispatcher::update(HttpResponse http_response) {
    Publisher<HttpResponse>::notifySubscribers(http_response);
}
