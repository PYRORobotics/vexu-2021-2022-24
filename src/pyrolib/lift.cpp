//
// Created by mayowa on 4/19/22.
//

#include "../../include/pyrolib/include/lift.h"
#include <utility>

namespace pyro {
    lift::lift(okapi::MotorGroup lift_motors, double raised_pos, double lowered_pos, lift_state initial_state) :
            lift_motors(std::move(lift_motors)),
            raised_pos(raised_pos),
            lowered_pos(lowered_pos) {
        lift_motors.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
        lift_motors.tarePosition();
        state = initial_state;
    }

    void lift::toggle() {
        if (state == LOWERED) {
            raise();
        } else {
            lower();
        }
    }

    void lift::raise_task() {
        lift_motors.setCurrentLimit(2500);
        lift_motors.moveAbsolute(raised_pos, 300);
        pros::delay(40);
        state = RAISED;
    }

    void lift::lower_task() {
        lift_motors.moveAbsolute(lowered_pos, 300);
        pros::delay(40);
        state = LOWERED;
    }

    void lift::raise() {
        pros::Task raise_task([this] { this->raise_task(); });
    }

    void lift::lower() {
        pros::Task lower_task([this] { this->lower_task(); });
    }
}