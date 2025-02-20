#pragma once

#include "notifier.hpp"

class System : public Notifier<> {
  public:
    void next() { Notifier<>::notify(); }
};
