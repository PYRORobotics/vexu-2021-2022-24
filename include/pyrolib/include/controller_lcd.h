//
// Created by charles on 1/9/20.
//

#ifndef VEXU_2021_2022_24_CONTROLLER_LCD_H
#define VEXU_2021_2022_24_CONTROLLER_LCD_H

namespace pyro {
    class controllerLCD {
    public:
        controllerLCD(pros::Controller &controller);

        [[noreturn]] void controllerLCDLoop();

        void setControllerLCD(int line, std::string linedataString);

    private:
        std::string prepareStringForLCD(std::string input);

        pros::Task t;

        std::string LCDLines[4];
        pros::Controller controller;
    };

    extern controllerLCD masterLCD;
    extern controllerLCD partnerLCD;
}

#endif //VEXU_2021_2022_24_CONTROLLER_LCD_H
