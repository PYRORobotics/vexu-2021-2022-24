//
// Created by charles on 3/3/22.
//

#ifndef VEXU_2021_2022_15_DEVICES_H
#define VEXU_2021_2022_15_DEVICES_H

#include "main.h"

// controller related
inline okapi::Controller master(okapi::ControllerId::master);

inline okapi::ControllerButton main_lift_btn(okapi::ControllerDigital::up);
inline okapi::ControllerButton side_lift_btn(okapi::ControllerDigital::right);
inline okapi::ControllerButton back_lift_btn(okapi::ControllerDigital::down);

inline okapi::ControllerButton main_jaw_btn(okapi::ControllerDigital::X);
inline okapi::ControllerButton side_jaw_btn(okapi::ControllerDigital::A);
inline okapi::ControllerButton back_jaw_btn(okapi::ControllerDigital::B);

inline okapi::ControllerButton intake_btn(okapi::ControllerDigital::L2);

inline pros::Controller prosMaster(pros::E_CONTROLLER_MASTER);

inline pyro::controllerLCD masterLCD(prosMaster);

// motor/robot related
inline pyro::chassis chassis(
        {-17, 18, -19},    // Left motors are 3 & 4
        {3, -20, 21},     // Right motors are 1 & 2 (reversed)
        1.5
);

inline pyro::intake intake(-6);

inline pyro::lift main_lift({13, -14}, 2400, 0, pyro::lift::LOWERED);
inline pyro::lift side_lift({7}, 0, -16500, pyro::lift::RAISED);
inline pyro::lift back_lift({16}, -2000, -5800, pyro::lift::RAISED);

inline pros::ADIDigitalIn jaws1Trigger('B');
inline pyro::jaws main_jaws(11, jaws1Trigger, 360);



//inline pros::Imu imu(14);

//inline pros::ADIDigitalIn jaws2Trigger('G');
//inline pyro::jaws jaws2(19, jaws2Trigger, 370);

//inline okapi::ADIEncoder encoderLeft{'A', 'B', true};
//inline okapi::ADIEncoder encoderRight{'C', 'D', true};

#endif //VEXU_2021_2022_15_DEVICES_H
