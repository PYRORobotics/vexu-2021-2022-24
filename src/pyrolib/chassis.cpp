//
// Created by charles on 9/30/21.
//

#include "pyrolib/include/chassis.h"

#include <memory>
namespace pyro {
    std::shared_ptr<okapi::ChassisController> chassis::getChassisController(){
        return this->chassisController;
    }

    void chassis::arcade(double forward, double turn, double threshold) {
        chassisController->getModel()->arcade(forward, turn, threshold);
    }

    void chassis::strafe(double power){
        okapi::Motor(9).setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
        okapi::Motor(9).moveVoltage(power * 12000.0);
    }

    void chassis::tank(double left, double right, double threshold) {
        chassisController->getModel()->tank(left, right, threshold);
    }

    void chassis::setCurrentLimit(int milliamps){
        for(okapi::Motor motor : leftMotorsVector){
            motor.setCurrentLimit(milliamps);
        }
        for(okapi::Motor motor : rightMotorsVector){
            motor.setCurrentLimit(milliamps);
        }
    }

    chassis::chassis(std::initializer_list<okapi::Motor> leftMotorsList,
                           std::initializer_list<okapi::Motor> rightMotorsList,
                           double gearRatio,
                           okapi::QLength wheelDiameter,
                           okapi::QLength wheelWidth,
                           const std::int32_t TPR) {
        leftMotorsVector = leftMotorsList;
        rightMotorsVector = rightMotorsList;

        leftMotors = std::make_shared<okapi::MotorGroup>(leftMotorsList);
        rightMotors = std::make_shared<okapi::MotorGroup>(rightMotorsList);

        chassisController = okapi::ChassisControllerBuilder()
                .withMotors(
                        leftMotorsList, // Left motors are 1 & 2 (reversed)
                        rightMotorsList    // Right motors are 3 & 4
                )
                /*
                .withSensors(
                        okapi::ADIEncoder{'A', 'B', true}, // Left encoder in ADI ports A & B
                        okapi::ADIEncoder{'C', 'D', true}  // Right encoder in ADI ports C & D (reversed)
                )
                .withGains(
                        {0.001, 0, 0.0001}, // Distance controller gains
                        {0.005, 0, 0.0002}  // Turn controller gains
                )*/
                //.withMaxVelocity(30)
                //.withDimensions({okapi::AbstractMotor::gearset::green,gearRatio}, {{2.8_in, 5.9_in}, 360})
                //.withDimensions({okapi::AbstractMotor::gearset::green,0.41}, {{2.75_in, 5.9_in}, 360})
                .withDimensions({okapi::AbstractMotor::gearset::green, gearRatio}, {{wheelDiameter, wheelWidth}, static_cast<double>(TPR)})
                .build();

        profileController =
                okapi::AsyncMotionProfileControllerBuilder()
                        .withLimits({
                                            3.0,
                                            //0.5, // Maximum linear velocity of the Chassis in m/s
                                            0.8, // Maximum linear acceleration of the Chassis in m/s/s
                                            100.0 // Maximum linear jerk of the Chassis in m/s/s/s
                                    })
                        .withOutput(chassisController)
                        .buildMotionProfileController();
    }

}
