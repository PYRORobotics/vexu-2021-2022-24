//
// Created by mayowa on 4/20/22.
//

#include "../../include/pyrolib/include/intake.h"
#include <utility>

namespace pyro {
    intake::intake(okapi::Motor intake_motor) : intake_motor(std::move(intake_motor)) {
        intake_motor.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
        state = STOPPED;
    }

    void intake::toggle() {
        if(state == RUNNING) {
            intake_motor.moveVoltage(0);
            intake_motor.setCurrentLimit(0);
            state = STOPPED;
        } else {
            intake_motor.setCurrentLimit(2500);
            intake_motor.moveVelocity(60);
            state = RUNNING;
        }
    }
}