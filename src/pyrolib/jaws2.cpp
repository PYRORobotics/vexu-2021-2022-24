//
// Created by charles on 1/12/22.
//

#include "../../include/pyrolib/include/jaws2.h"

#include <utility>
namespace pyro {
    jaws2::jaws2(okapi::Motor jawsMotor, pros::ADIDigitalIn limit, pros::ADIDigitalIn jawsTrigger, double target_pos) : jawsMotor(std::move(jawsMotor)),
                                                                                                  jawsTrigger(jawsTrigger),
                                                                                                  limit(limit),
                                                                                                  state(UNKNOWN),
                                                                                                  triggered(false), open_pos(target_pos){
        jawsMotor.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
        jawsMotor.setGearing(okapi::AbstractMotor::gearset::red);
        jawsMotor.setEncoderUnits(okapi::AbstractMotor::encoderUnits::degrees);
        state = CLOSED;
    }

    bool jaws2::calibrate_task() {
        jawsMotor.moveVelocity(30);
        pros::delay(50);
        while(jawsMotor.getCurrentDraw() > 100) {
            pros::delay(10);
        }
        jawsMotor.moveVoltage(-2000);
        pros::delay(150);
        while(abs(jawsMotor.getActualVelocity()) > 1){
            pros::delay(10);
        }
        pros::delay(500);
        jawsMotor.moveVoltage(0);

        jawsMotor.tarePosition();
        state = CALIBRATED;
        return true;
    }

    bool jaws2::calibrate() {
        pros::Task cal([this]{ this->calibrate_task(); });
        return true;
    }

    bool jaws2::open_task() {
        if(state == CALIBRATED || state == CLOSED){
//            int absolute_pos = jawsMotor.getPosition();
//            int revs = (int) absolute_pos / 360;
//            int relative_pos = (int) absolute_pos % 360;
//            if(relative_pos > 50){
//                revs++;
//            }
//            int target_pos = open_pos + (revs * 360);
//            printf("abs_pos: %d\n", absolute_pos);
//            printf("revs: %d\n", revs);
//            printf("relative_pos: %d\n", relative_pos);
//            printf("target_pos: %d\n", target_pos);
//            jawsMotor.moveAbsolute(target_pos, 100);
//            pros::delay(40);
//            while(abs(jawsMotor.getActualVelocity()) > 0){
//                pros::delay(10);
//            }
            while(!limit.get_value()) {
                jawsMotor.moveVelocity(90);
                pros::delay(10);
            }

            jawsMotor.moveVelocity(0);
            jawsMotor.moveRelative(20, 100);
            state = OPEN;
            return true;
        }
        else{
            return false;
        }
    }

    bool jaws2::open() {
        pros::Task ope([this]{ this->open_task(); });
        return true;
    }

    bool jaws2::close_task() {
//        if(state == OPEN) {
//            jawsMotor.moveRelative(80, 100);
//            pros::delay(500);
//            jawsMotor.moveVoltage(-2000);
//            state = CLOSED;
//            return true;
//        }
//        else{
//            return false;
//        }
        if(state == OPEN) {/*
            long opened_draw = jawsMotor.getCurrentDraw();

            while(opened_draw - jawsMotor.getCurrentDraw() < 200) {
                pros::delay(200);
                jawsMotor.moveVelocity(100);
                printf("%ld\n", jawsMotor.getCurrentDraw());
            }
            jawsMotor.moveVelocity(0);
*/
            jawsMotor.moveRelative(100, 100);
            pros::delay(500);
            jawsMotor.moveVoltage(-2000);
            state = CLOSED;
            return true;
        }
        else{
            return false;
        }
    }

    void jaws2::toggle() {
        if(state == OPEN) {
            close();
        } else if(state == CLOSED || state == CALIBRATED) {
            open();
        }
    }

    bool jaws2::close() {
        pros::Task clo([this]{ this->close_task(); });
        return true;
    }

    bool jaws2::isTriggered() {
        triggered = !jawsTrigger.get_value();
        return triggered;
    }

    bool jaws2::getNewTrigger() {
        //printf("button: %d, triggered: %d\n", jawsTrigger.get_value(), triggered);
        return jawsTrigger.get_new_press();
        if(!triggered && isTriggered()){
            return true;
        }
        else {
            return false;
        }
    }

    double jaws2::getTemperature() {
        return jawsMotor.getTemperature();
    }

    double jaws2::getPosition(){
        return jawsMotor.getPosition();
    }


}
