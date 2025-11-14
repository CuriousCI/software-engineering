#pragma once

#include <random>

std::default_random_engine pseudo_random_engine_from_device();

const size_t ITERATIONS = 10000;
const size_t SIMULATION_HORIZON = 10000;
