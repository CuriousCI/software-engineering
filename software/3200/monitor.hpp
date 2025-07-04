#pragma once

#include "../../mocc/recorder.hpp"
#include "parameters.hpp"

class Monitor : public Recorder<NetworkPayloadLight>,
                public Recorder<Light>,
                public Observer<> {

    int time = 0;
    bool messages_lost = false;

  public:
    Monitor() : Recorder<NetworkPayloadLight>(RED), Recorder<Light>(RED) {}

    void update() override {
        if (Recorder<NetworkPayloadLight>::record != Recorder<Light>::record)
            time++;
        else
            time = 0;

        if (time > 3)
            messages_lost = true;
    }

    bool is_valid() { return !messages_lost; }
};
