//
// Created by charles on 3/3/22.
//

#ifndef VEXU_2021_2022_15_DEVICES_H
#define VEXU_2021_2022_15_DEVICES_H

#include "main.h"

// controller related
inline okapi::Controller master(okapi::ControllerId::master);
inline okapi::Controller partner(okapi::ControllerId::partner);

inline okapi::ControllerButton main_lift_btn(okapi::ControllerDigital::up);
inline okapi::ControllerButton side_lift_btn(okapi::ControllerDigital::right);
inline okapi::ControllerButton back_lift_btn(okapi::ControllerDigital::down);

inline okapi::ControllerButton main_jaw_btn(okapi::ControllerDigital::X);
inline okapi::ControllerButton side_jaw_btn(okapi::ControllerDigital::A);
inline okapi::ControllerButton back_jaw_btn(okapi::ControllerDigital::B);

inline okapi::ControllerButton intake_btn(okapi::ControllerDigital::L2);

inline pros::Controller prosMaster(pros::E_CONTROLLER_MASTER);
inline pros::Controller prosPartner(pros::E_CONTROLLER_PARTNER);

inline pyro::controllerLCD masterLCD(prosMaster);

// motor/robot related
inline pyro::chassis chassis(
        {-17, 18, -19},    // Left motors are 3 & 4
        {3, -20, 21},     // Right motors are 1 & 2 (reversed)
        //1.5
        1.0,
        4.125_in,
        15_in
);

inline pyro::intake intake(-6);

inline okapi::Motor mtr_left(13, false, okapi::AbstractMotor::gearset::red,
                      okapi::AbstractMotor::encoderUnits::degrees);
inline okapi::Motor mtr_right(14, true, okapi::AbstractMotor::gearset::red,
                       okapi::AbstractMotor::encoderUnits::degrees);

inline pyro::lift main_lift(
        {mtr_left, mtr_right},
        (1.0 / 6.0),
        90, 0,
        pyro::lift::LOWERED
);

inline okapi::Motor side_lift_mtr(7, false, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees);

// wip, needs updating
inline pyro::lift side_lift(
        {side_lift_mtr},
        (1.0 / 5.0),
        450, 0,
        pyro::lift::LOWERED
);

inline okapi::Motor back_lift_mtr(16, false, okapi::AbstractMotor::gearset::red, okapi::AbstractMotor::encoderUnits::degrees);

// wip, needs updating
inline pyro::lift back_lift(
        {back_lift_mtr},
        (1.0 / 5.0),
        -23, -240,
        pyro::lift::RAISED
);

inline pros::ADIDigitalIn jaws1Trigger('B');
inline pyro::jaws main_jaws(11, jaws1Trigger, 415);

inline pros::ADIDigitalIn jaws2Trigger('A');
inline pyro::jaws side_jaws(10, jaws2Trigger, 415);

inline pros::ADIDigitalIn jaws3Trigger('C');
inline pyro::jaws back_jaws(15, jaws3Trigger, 385);



//inline pros::Imu imu(14);

//inline pros::ADIDigitalIn jaws2Trigger('G');
//inline pyro::jaws jaws2(19, jaws2Trigger, 370);

//inline okapi::ADIEncoder encoderLeft{'A', 'B', true};
//inline okapi::ADIEncoder encoderRight{'C', 'D', true};

#endif //VEXU_2021_2022_15_DEVICES_H
