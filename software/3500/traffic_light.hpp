#pragma once

#include "../../mocc/notifier.hpp"
#include "../../mocc/observer.hpp"
#include "parameters.hpp"
#include <cstdlib>

class TrafficLight : public Observer<Fault>,
                     public Observer<Message>,
                     public Notifier<Light> {
    Light l = Light::RED;

  public:
    void update(Message message) override {
        l = message;
        notify(l);
    }

    void update(Fault) override {
        l = Light::RED;
        notify(l);
    }

    Light light() { return l; }
};
