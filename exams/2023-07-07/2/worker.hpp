#pragma once

#include "../../../mocc/buffer.hpp"
#include "parameters.hpp"

#define IDLE 0

/* TODO */
class Worker : public Buffer<Task>, public Observer<> {
    size_t status = IDLE;

  public:
    Worker() : Buffer(B) {}

    void update() {}
};
