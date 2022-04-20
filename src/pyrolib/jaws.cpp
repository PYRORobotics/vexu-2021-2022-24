//
// Created by charles on 1/12/22.
//

#include "../../include/pyrolib/include/jaws.h"

#include <utility>
namespace pyro {
    jaws::jaws(okapi::Motor jawsMotor, pros::ADIDigitalIn jawsTrigger, double target_pos) : jawsMotor(std::move(jawsMotor)),
                                                                                                  jawsTrigger(jawsTrigger),
                                                                                                  state(UNKNOWN),
                                                                                                  triggered(false), open_pos(target_pos){
        jawsMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
        jawsMotor.setGearing(okapi::AbstractMotor::gearset::red);
        jawsMotor.setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
        state = CLOSED;
    }

    bool jaws::calibrate_task() {
        jawsMotor.moveVelocity(30);
        pros::delay(50);
        while(jawsMotor.getCurrentDraw() > 100){
            pros::delay(10);
        }
        jawsMotor.moveVoltage(-3000);
        pros::delay(150);
        while(abs(jawsMotor.getActualVelocity()) > 1){
            pros::delay(10);
        }
        pros::delay(500);
        jawsMotor.moveVoltage(0);
        int absolute_pos = jawsMotor.getPosition();
        int relative_pos = (int) absolute_pos % 360;
        printf("old_rel_pos: %d\n", relative_pos);
        jawsMotor.tarePosition();
        state = CALIBRATED;
        return true;
    }

    bool jaws::calibrate() {
        pros::Task cal([this]{ this->calibrate_task(); });
        return true;
    }

    bool jaws::open_task() {
        if(state == CALIBRATED || state == CLOSED){
            int absolute_pos = jawsMotor.getPosition();
            int revs = (int) absolute_pos / 360;
            int relative_pos = (int) absolute_pos % 360;
            if(relative_pos > 50){
                revs++;
            }
            int target_pos = open_pos + (revs * 360);
            printf("abs_pos: %d\n", absolute_pos);
            printf("revs: %d\n", revs);
            printf("relative_pos: %d\n", relative_pos);
            printf("target_pos: %d\n", target_pos);
            jawsMotor.moveAbsolute(target_pos, 100);
            pros::delay(40);
            while(abs(jawsMotor.getActualVelocity()) > 0){
                pros::delay(10);
            }
            state = OPEN;
            return true;
        }
        else{
            return false;
        }
    }

    bool jaws::open() {
        pros::Task ope([this]{ this->open_task(); });
        return true;
    }

    bool jaws::close_task() {
        if(state == OPEN) {
            jawsMotor.moveRelative(60, 100);
            pros::delay(500);
            jawsMotor.moveVoltage(-2000);
            state = CLOSED;
            return true;
        }
        else{
            return false;
        }
    }

    bool jaws::close() {
        pros::Task clo([this]{ this->close_task(); });
        return true;
    }

    bool jaws::isTriggered() {
        triggered = !jawsTrigger.get_value();
        return triggered;
    }

    bool jaws::getNewTrigger() {
        //printf("button: %d, triggered: %d\n", jawsTrigger.get_value(), triggered);
        return jawsTrigger.get_new_press();
        if(!triggered && isTriggered()){
            return true;
        }
        else {
            return false;
        }
    }

    double jaws::getTemperature() {
        return jawsMotor.getTemperature();
    }

    double jaws::getPosition(){
        return jawsMotor.getPosition();
    }


}
