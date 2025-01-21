#ifndef DATA_HPP_
#define DATA_HPP_

#include <cstddef>

#include "../case/alias.hpp"
#include "../case/case.hpp"

struct Data {
    size_t id, f;
    real_t time;
};

struct Resp {
    bool is_confirmed;
    real_t time;
};

ALIAS_TYPE(HttpRequest, Data)
ALIAS_TYPE(HttpResponse, Resp)

ALIAS_TYPE(Query, Data)
ALIAS_TYPE(Reply, Resp)

#endif
