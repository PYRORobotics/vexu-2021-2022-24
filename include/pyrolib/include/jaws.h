//
// Created by charles on 1/12/22.
//

#ifndef VEXU_2021_2022_TESTBED_JAWS_H
#define VEXU_2021_2022_TESTBED_JAWS_H
#include "../../main.h"

namespace pyro {
    class jaws {
    public:
        jaws(okapi::Motor jawsMotor, pros::ADIDigitalIn jawsTrigger, double target_pos = 380);

        bool calibrate();
        bool close();
        bool open();
        bool isTriggered();
        bool getNewTrigger();
        double getTemperature();
        double getPosition();


    private:
        okapi::Motor jawsMotor;
        pros::ADIDigitalIn jawsTrigger;
        enum {
            UNKNOWN,
            CALIBRATED,
            CLOSED,
            OPEN
        } typedef jawsState;
        jawsState state;
        bool triggered;

        bool calibrate_task();

        bool open_task();

        bool close_task();

        double open_pos;

    };
}


#endif //VEXU_2021_2022_TESTBED_JAWS_H
