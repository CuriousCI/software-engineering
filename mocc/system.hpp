#pragma once

#include "notifier.hpp"
#include "observer.hpp"

using SystemObserver = Observer<>;

/* An object used to synchronize a set of entities.
 * Other entities can connect to it either directly or via stopwatches or
 * timers.
 * */
class System : public Notifier<> {
  public:
    /* Simulates one step of the system. */
    void next() { Notifier<>::notify(); }
};
