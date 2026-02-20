#include "main.h"
#include "lemlib/api.hpp"
#include "subsystems.hpp"

int intakeState = intakeStates::IDLE;

void setIntake(int speed)
{
    intake.move_voltage(speed);
}

void setPistons(bool tri1, bool tri2, bool shared)
{
        tristate1.set_value(tri1);
        tristate2.set_value(tri2);
        outtakesharing.set_value(shared);
}

void intakeStateMachine() {
    if (intakeState == IDLE) {

        setIntake(0);
        setPistons(false, false, false);

    } else if (intakeState == INTAKING) {

        setIntake(INTAKE_SPEED);
        setPistons(false, false, false);

    } else if (intakeState == OUTTAKING) {

        setIntake(OUTTAKE_SPEED);
        setPistons(false, false, true);

    } else if (intakeState == MID_SCORE) {

        setIntake(MID_SPEED);
        setPistons(true, false, false);

    } else if (intakeState == HIGH_SCORE) {

        setIntake(INTAKE_SPEED);
        setPistons(true, true, false);

    }

}
void intakeTelemetry() {

    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeState = intakeStates::INTAKING;
    }

    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeState = intakeStates::OUTTAKING;
    }

    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intakeState = intakeStates::HIGH_SCORE;
    }

    else if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intakeState = intakeStates::MID_SCORE;
    }
    else {
        intakeState = intakeStates::IDLE;
    }
}

bool midState = false;

void midControl(bool state)
{
    outtakesharing.set_value(state);
}

void midTeleControl()
{
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
    {
        midState = !midState;
    }
    midControl(midState);
}

void wingControl(bool state)
{
    wing.set_value(state);
}

bool wingState = false;

void wingTeleControl()
{
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        wingState = !wingState;
    }
    wingControl(wingState);
}

void loaderControl(bool state)
{
    flipdown.set_value(state);
}

bool loaderState = false;

void loaderTeleControl()
{
    if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
    {
        loaderState = !loaderState;
    }
    loaderControl(loaderState);
}


// auton functions below

void setAutoIntake(int state)
{
    intakeState = state;
}