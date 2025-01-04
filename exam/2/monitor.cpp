#include "monitor.hpp"

void Monitor::update(HttpRequest http_request) {
    interval.save(((Data)http_request).time - last_request_time);
    last_request_time = ((Data)http_request).time;
}
