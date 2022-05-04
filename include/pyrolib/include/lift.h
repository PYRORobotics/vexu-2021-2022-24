//
// Created by mayowa on 4/19/22.
//

#ifndef VEXU_2021_2022_24_LIFT_H
#define VEXU_2021_2022_24_LIFT_H

#include "../../main.h"

namespace pyro {
    class lift {
    public:
        enum lift_state {
            RAISED,
            LOWERED
        };

        lift(std::shared_ptr<okapi::MotorGroup> lift_motors, double gear_ratio, double raised_pos, double lowered_pos, lift_state initial_state);

        void toggle();

        std::shared_ptr<okapi::MotorGroup> lift_motors;
        void lower();
        void raise();


    private:

        lift_state state;



        void raise_task();

        void lower_task();

        double gear_ratio;
        double raised_pos;
        double lowered_pos;

    };
}
#endif //VEXU_2021_2022_24_LIFT_H
