#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "subsystems.hpp"

int intakeState = intakeStates::IDLE;

//pistons for the flipdown, wing, and tristate

void setPistons(bool tri1, bool tri2) {
        tristate1.set_value(tri1);
        tristate2.set_value(tri2);
}

void intakeStateMachine() {
    if (intakeState == IDLE) {

        setIntake(0);
        setPistons(true, false);

    } else if (intakeState == INTAKING) {

        setIntake(127);
        setPistons(true, false);

    } else if (intakeState == OUTTAKING) {

        setIntake(-127);
        setPistons(true, false);

    } else if (intakeState == HIGH_SCORE) {

        setIntake(127);
        setPistons(false, false);

    } else if (intakeState == MID_SCORE) {

        setIntake(127);
        setPistons(true, true);

    }

}
void intakeTelemetry() {

    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
        intakeState = intakeStates::HIGH_SCORE;
    }

    else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
        intakeState = intakeStates::MID_SCORE;
    }

    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
        intakeState = intakeStates::INTAKING;
    }

    else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
        intakeState = intakeStates::OUTTAKING;
    }
    
    else {
        intakeState = intakeStates::IDLE;
    }
}


// auton functions below

void setIntake(int state)
{
    intakeState = state;
}