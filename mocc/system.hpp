#pragma once

#include "notifier.hpp"
#include "observer.hpp"

using SystemObserver = Observer<>;

/* It is used to synchronize all the entities in a system. */
/* Other entities can connect to it either directly or via stopwatches or
 * timers. */
class System : public Notifier<> {
  public:
    /* Simulates one step of the system. */
    void next() { Notifier<>::notify(); }
};
