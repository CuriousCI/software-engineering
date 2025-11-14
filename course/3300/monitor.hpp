#pragma once

#include "../../mocc/recorder.hpp"
#include "parameters.hpp"

class Monitor : public Recorder<NetworkPayloadLight>,
                public Recorder<Light>,
                public Observer<> {

    int time_value_is_invalid = 0;
    bool messages_lost = false;

  public:
    Monitor() : Recorder<NetworkPayloadLight>(RED), Recorder<Light>(RED) {}

    void update() override {
        if (Recorder<NetworkPayloadLight>::record != Recorder<Light>::record)
            time_value_is_invalid++;
        else
            time_value_is_invalid = 0;

        if (time_value_is_invalid > 3)
            messages_lost = true;
    }

    bool isValid() { return !messages_lost; }
};
