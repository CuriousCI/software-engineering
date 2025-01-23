#ifndef MOCC_SYSTEM_HPP_
#define MOCC_SYSTEM_HPP_

#include "notifier.hpp"

/*, public Notifier<System *> */
/*Notifier<System *>::notify(this);*/

class System : public Notifier<> {
  public:
    void next() { Notifier<>::notify(); }
};

#endif
