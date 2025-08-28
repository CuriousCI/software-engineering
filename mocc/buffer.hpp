#pragma once

#include "observer.hpp"
#include <deque>
#include <exception>

/* Is the buffer has a limit and the limit is reached then an exception is
 * thrown.
 */
class buffer_full : public std::exception {};

/* A buffer is an entity that stores items in a queue while the items wait for
 * their turn to be processed.
 * */
template <typename T> class Buffer : public Observer<T> {
  protected:
    size_t limit;
    std::deque<T> buffer;

  public:
    /* A buffer with limit 0 is 'unlimited' (it can have infinite items). */
    Buffer(size_t limit = 0) { this->limit = limit; };

    /* It is easier to use a Buffer<T> instead of a std::deque<T> because it
     * already implements update().
     * */
    void update(T item) override {
        if (limit > 0 && buffer.size() > limit)
            throw buffer_full();

        buffer.push_back(item);
    }
};
