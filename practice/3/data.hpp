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

const real_t tau = 2, rho = 20;
const size_t N = 300, F = 10, K = 10, S = 10, Q = 100, HORIZON = 100000;

#endif
