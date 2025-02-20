#pragma once

#include "observer.hpp"
#include <deque>
#include <exception>

class buffer_full : public std::exception {};

template <typename T> class Buffer : public Observer<T> {
  protected:
    size_t limit = 0;
    std::deque<T> buffer;

  public:
    Buffer(size_t limit = 0) { this->limit = limit; };

    void update(T args) override {
        if (limit > 0 && buffer.size() > limit)
            throw buffer_full();

        buffer.push_back(args);
    }
};
