//
// Created by mayowa on 4/20/22.
//

#ifndef VEXU_2021_2022_24_INTAKE_H
#define VEXU_2021_2022_24_INTAKE_H

#include "../../main.h"

namespace pyro {
    class intake {
    public:
        intake(okapi::Motor intake_motor);

        void toggle();

    private:
        okapi::Motor intake_motor;

        enum {
            RUNNING,
            STOPPED
        } typedef intake_state;

        intake_state state;
    };
}

#endif //VEXU_2021_2022_24_INTAKE_H
