#pragma once
#include "observer.hpp"

/* This is a boilerplate class which proved to be useful quite often.
 * When a recorder recieves a notification it stores its value temporarely in
 * the "record" attribute.
 * */
template <typename T> class Recorder : public Observer<T> {
  protected:
    T record;

  public:
    Recorder() {}

    /* An initial value can be set for the recorder.
     * It's useful because sometimes the recorder needs some default value to
     * use in case it hasn't received any notification yet.
     * */
    Recorder(T record) : record(record) {}

    /* Stores a notification temporarely. */
    void update(T args) override { record = args; }

    /* The value saved in the recorder can be accessed directly without going
     * through the ".record" attribute.
     * */
    operator T() const { return record; }
};
