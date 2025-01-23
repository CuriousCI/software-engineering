#ifndef MOCC_READER_HPP_
#define MOCC_READER_HPP_

#include "observer.hpp"

template <typename T> class Recorder : public Observer<T> {
  protected:
    T record;

  public:
    Recorder() {}
    Recorder(T record) : record(record) {}

    void update(T args) override { record = args; }

    operator T() const { return record; }
};

#endif
