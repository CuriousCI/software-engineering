#ifndef TYPE_HPP_
#define TYPE_HPP_

#include "../../../mocc/alias.hpp"
#include "../../../mocc/mocc.hpp"
#include <cstddef>

const size_t T = 1, HORIZON = 1000000;

struct Request {
    real_t t;
};

STRONG_ALIAS(CustomerId, size_t);
STRONG_ALIAS(RequestCount, size_t);

#endif
