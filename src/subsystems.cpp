#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems.hpp"

int intakeState = intakeStates::IDLE;

void setIntake(int speed) 
{
    intake.move_voltage(speed);
}

//pistons for the flipdown, wing, and tristate

void setPistons(bool tri1, bool tri2, bool shared) {
        tristate1.set_value(tri1);
        tristate2.set_value(tri2);
        outtakesharing.set_value(shared);
}

void intakeStateMachine() {
    if (intakeState == IDLE) {

        setIntake(0);
        setPistons(true, false, false);

    } else if (intakeState == INTAKING) {

        setIntake(12000);
        setPistons(true, false, false);

    } else if (intakeState == OUTTAKING) {

        setIntake(-12000);
        setPistons(true, false, true);

    } else if (intakeState == HIGH_SCORE) {

        setIntake(12000);
        setPistons(false, false, false);

    } else if (intakeState == MID_SCORE) {

        setIntake(12000);
        setPistons(true, true, false);

    }

}
void intakeTelemetry() {

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeState = intakeStates::INTAKING;
    }

    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeState = intakeStates::OUTTAKING;
    }

    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intakeState = intakeStates::HIGH_SCORE;
    }

    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intakeState = intakeStates::MID_SCORE;
    }
    else {
        intakeState = intakeStates::IDLE;
    }
}


// auton functions below

void setAutoIntake(int state)
{
    intakeState = state;
}