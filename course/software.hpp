#include "../mocc/mocc.hpp"

static std::random_device device_randomness_source;
static urng_t urng(device_randomness_source());
