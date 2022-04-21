//
// Created by mayowa on 4/19/22.
//

#include "../../include/pyrolib/include/lift.h"
#include <utility>

namespace pyro {
    lift::lift(okapi::MotorGroup lift_motors,
               double gear_ratio,
               double raised_pos, double lowered_pos,
               lift_state initial_state
    ) :
            lift_motors(std::move(lift_motors)),
            gear_ratio(gear_ratio),
            raised_pos(raised_pos),
            lowered_pos(lowered_pos) {
        // wip, problem: calling methods on lift_motors don't seem to update the motor
        // warning message: 'lift_motors' used after it was moved
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
        double calculated_pos = (1 / gear_ratio) * raised_pos;
        printf("%lf", calculated_pos);
        lift_motors.moveAbsolute(calculated_pos, 300);
        pros::delay(40);
        state = RAISED;
    }

    void lift::lower_task() {
        lift_motors.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
        double calculated_pos = (1 / gear_ratio) * lowered_pos;
        lift_motors.moveAbsolute(calculated_pos, 300);
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