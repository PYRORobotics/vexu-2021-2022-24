//
// Created by charles on 9/30/21.
//
#ifndef VEXU_2021_2022_TESTBED_CHASSIS_H
#define VEXU_2021_2022_TESTBED_CHASSIS_H

#include "../../main.h"

using namespace okapi::literals;


namespace pyro {
    class chassis {
    public:
        chassis(std::initializer_list<okapi::Motor> leftMotorsList,
                std::initializer_list<okapi::Motor> rightMotorsList,
                double gearRatio = 1.0,
                okapi::QLength wheelDiameter = 4_in,
                okapi::QLength wheelWidth = 11.5_in,
                std::int32_t TPR = okapi::imev5GreenTPR);

        void arcade(double forward, double turn, double threshold = 0.05);

        void tank(double left, double right, double threshold = 0.05);

        void setCurrentLimit(int milliamps);

        std::shared_ptr<okapi::ChassisController> chassisController;
        std::shared_ptr<okapi::ChassisController> getChassisController();
        std::shared_ptr<okapi::AsyncMotionProfileController> profileController;

    private:
        std::vector<okapi::Motor> leftMotorsVector;
        std::vector<okapi::Motor> rightMotorsVector;
        std::shared_ptr<okapi::MotorGroup> leftMotors;
        std::shared_ptr<okapi::MotorGroup> rightMotors;

    };
}


#endif //VEXU_2021_2022_TESTBED_CHASSIS_H
