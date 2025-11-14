#pragma once

#include "../../mocc/notifier.hpp"
#include "../../mocc/observer.hpp"
#include "parameters.hpp"
#include <cstdlib>

class TrafficLight : public Observer<LightUpdateMessage>,
                     public Notifier<Light> {
    Light l = Light::RED;

  public:
    void update(LightUpdateMessage light) override {
        l = light;
        notify(l);
    }

    Light light() { return l; }
};
