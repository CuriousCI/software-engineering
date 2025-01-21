#include "monitor.hpp"

Monitor::Monitor(const size_t id) : id(id) {}

void Monitor::update(HttpResponse http_response) {
    response_time.save(Reader<Time>::value - ((Resp)http_response).time);
}
