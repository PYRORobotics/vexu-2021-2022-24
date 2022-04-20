#include "main.h"
#include "devices.h"


using namespace okapi;
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);


    leftLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    rightLift.setBrakeMode(AbstractMotor::brakeMode::hold);
    //back.setEncoderUnits(AbstractMotor::encoderUnits::degrees);
    //back.setGearing(okapi::AbstractMotor::gearset::red);

    chassis.chassisController->getModel()->setBrakeMode(AbstractMotor::brakeMode::hold);

    pros::delay(50);
    /*while(imu.is_calibrating()){
        pros::delay(20);
    }*/

    /*
    chassis.profileController->generatePath({
                                            {0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
                                            {28_in, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
                                    "forwardGoal" // Profile name
    );*/

    chassis.profileController->generatePath({
                                                    {0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
                                                    {18.5_in, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
                                            "forwardGoal" // Profile name
    );

    chassis.profileController->generatePath({
                                                    {0_ft, 0_ft, 0_deg},  // Profile starting position, this will normally be (0, 0, 0)
                                                    {12_in, 0_ft, 0_deg}}, // The next point in the profile, 3 feet forward
                                            "forwardTest" // Profile name
    );
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

void startLift(){
    leftLift.moveAbsolute(400, 200);
    rightLift.moveAbsolute(400, 200);
    pros::delay(400);
    leftLift.moveVoltage(-4000);
    rightLift.moveVoltage(-4000);
};

int autonomousMode = 3;

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {

    if(autonomousMode == 0) {
        printf("start of auton pos: %f\n", jaws1.getPosition());
        jaws1.open();

        //pros::Task StartLift(startLift);

        printf("end of auton pos: %f\n", jaws1.getPosition());
        int runTime = 0;

        chassis.profileController->setTarget("forwardGoal");
        while (!jaws1.getNewTrigger() && runTime < 2000) {
            pros::delay(10);
            runTime += 10;
        }
        if(runTime >= 2000){
            masterLCD.setControllerLCD(0, "Auton Timeout");
        }

        jaws1.close();

        pros::delay(100);
        chassis.profileController->waitUntilSettled();

        chassis.profileController->setTarget("forwardGoal", true);
        pros::delay(100);
        chassis.profileController->waitUntilSettled();

        pros::delay(2000);
    }
    else if(autonomousMode == 1) {
        chassis.profileController->setTarget("forwardGoal");
        chassis.profileController->waitUntilSettled();
    }
    else if(autonomousMode == 2){
        chassis.chassisController->setMaxVelocity(100);
        printf("turning\n");
        chassis.chassisController->turnAngle(90_deg/0.37);
        printf("turning done\n");
        pros::delay(5000);
    }
    else if(autonomousMode == 3){
        printf("start of auton pos: %f\n", jaws1.getPosition());
        jaws1.open();

        //pros::Task StartLift(startLift);

        printf("end of auton pos: %f\n", jaws1.getPosition());
        int runTime = 0;

        chassis.profileController->setTarget("forwardGoal");
        while (!jaws1.getNewTrigger() && runTime < 2000) {
            pros::delay(10);
            runTime += 10;
        }
        if(runTime >= 2000){
            masterLCD.setControllerLCD(0, "Auton Timeout");
        }

        jaws1.close();

        pros::delay(100);
        chassis.profileController->waitUntilSettled();

        chassis.profileController->setTarget("forwardGoal", true);
        pros::delay(100);
        chassis.profileController->waitUntilSettled();

        pros::delay(2000);

        chassis.chassisController->setMaxVelocity(50);
        chassis.chassisController->turnAngle(50_deg);


        chassis.chassisController->setMaxVelocity(100);
        chassis.chassisController->moveDistance(1_ft);

        chassis.chassisController->setMaxVelocity(50);
        chassis.chassisController->turnAngle(-50_deg);


        chassis.chassisController->setMaxVelocity(100);
        chassis.chassisController->moveDistance(100_ft);



    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *stuff
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
[[noreturn]] void opcontrol() {
    //imu.tare();

    bool armOut = false;
    bool frontLiftUp = true;
    bool backLiftUp = true;
    double backPos = 0;
	while (true) {

        //masterLCD.setControllerLCD(0, "ctemp: " + std::to_string(jaws1.getTemperature()));

        //masterLCD.setControllerLCD(0, "degR: " + std::to_string(imu.get_roll()));
        //masterLCD.setControllerLCD(1, "degY: " + std::to_string(imu.get_yaw()));
        //masterLCD.setControllerLCD(0, "left: " + std::to_string((encoderLeft.get()/360.0)*PI*2.75));
        //masterLCD.setControllerLCD(1, "right: " + std::to_string((encoderRight.get()/360.0)*PI*2.75));
        masterLCD.setControllerLCD(0, "PYRO");
        masterLCD.setControllerLCD(1, "PYRO");
        masterLCD.setControllerLCD(2, "temp: " + std::to_string(jaws1.getTemperature()));


        //if(fabs(imu.get_roll()) > 33 && fabs(imu.get_roll()) < 360){
        if(false){
            //chassis.arcade((imu.get_roll() > 0) ? 1 : -1, 0, 0);
        }
        else {
            if (fabs(master.getAnalog(ControllerAnalog::leftY)) > 0.05 ||
                fabs(master.getAnalog(ControllerAnalog::rightX)) > 0.05 ||
                    fabs(master.getAnalog(ControllerAnalog::leftX)) > 0.05) {
                //printf("got here\n");
                chassis.setCurrentLimit(2500);
                chassis.arcade(master.getAnalog(ControllerAnalog::leftY),
                             master.getAnalog(ControllerAnalog::rightX), 0.05);
                chassis.strafe(master.getAnalog(ControllerAnalog::leftX));
            } else {
                //chassis.tank(0, 0, 0.05);
                //chassis.setCurrentLimit(0);
                chassis.chassisController->getModel()->stop();
                chassis.strafe(0);
            }

        }

        /*
        if(master.getDigital(ControllerDigital::L1)){
            backLift.setCurrentLimit(2500);
            frontLift.setCurrentLimit(2500);
            backLift.moveVelocity(200);
            frontLift.moveVelocity(200);
        }
        else if(master.getDigital(ControllerDigital::L2)){
            backLift.setCurrentLimit(2500);
            frontLift.setCurrentLimit(2500);
            backLift.moveVelocity(-200);
            frontLift.moveVelocity(-200);
        }
        else{
            backLift.setCurrentLimit(0);
            frontLift.setCurrentLimit(0);
            backLift.moveVelocity(0);
            frontLift.moveVelocity(0);
        }*/

        /*
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
            frontLiftUp = !frontLiftUp;
            if(frontLiftUp){
                frontLift.moveAbsolute(-50, 200);
            }
            else{
                frontLift.moveAbsolute(-4600, 200);
            }
        }
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
            backLiftUp = !backLiftUp;
            if(backLiftUp){
                backLift.moveAbsolute(-50, 200);
            }
            else{
                backLift.moveAbsolute(-4600, 200);
            }
        }*/


        if(master.getDigital(ControllerDigital::L1)){
            leftLift.setCurrentLimit(2500);
            leftLift.moveVelocity(200);

            rightLift.setCurrentLimit(2500);
            rightLift.moveVelocity(200);

        }
        else if(master.getDigital(ControllerDigital::L2)){
            leftLift.setCurrentLimit(2500);
            leftLift.moveVelocity(-200);

            rightLift.setCurrentLimit(2500);
            rightLift.moveVelocity(-200);
        }
        else{
            //rightLift.setCurrentLimit(0);
            //leftLift.setCurrentLimit(0);
            rightLift.moveVelocity(0);
            leftLift.moveVelocity(0);
        }

        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
            jaws1.calibrate();
        }
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
            jaws1.open();
        }
        if(jaws1.getNewTrigger() || prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)){
            jaws1.close();
        }

        /*
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
            jaws2.calibrate();
        }
        if(prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
            jaws2.open();
        }
        if(jaws2.getNewTrigger() || prosMaster.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){
            jaws2.close();
        }

        if(master.getDigital(okapi::ControllerDigital::down)){
            back.moveVelocity(-50);
            backPos = back.getPosition();
            if(backPos < -190){
                backPos = -190;
            }
        }
        else if(master.getDigital(okapi::ControllerDigital::up)){
            if(back.getPosition() < -5) {
                back.moveVelocity(50);
                backPos = back.getPosition();
            }
            else{
                back.moveVoltage(0);
            }
        }
        else{
            back.moveAbsolute(backPos, 50);
        }
         */
		pros::delay(10);
	}
}
