#pragma once
#include "lemlib/api.hpp" // IWYU pragma: keep

//intake motor group, ports 2 (forwards) and 9 (reversed), gearset is blue (600 rpm)
inline pros::MotorGroup intake({2,-9}, pros::MotorGearset::blue);

inline pros::adi::DigitalOut wing('A'); 
inline pros::adi::DigitalOut flipdown('B');
inline pros::adi::DigitalOut tristate1('C');
inline pros::adi::DigitalOut tristate2('D');
inline pros::adi::DigitalOut outtakesharing('E');

inline pros::Controller master(pros::E_CONTROLLER_MASTER);

inline const int INTAKE_SPEED = 12000;
inline const int OUTTAKE_SPEED = -12000;
inline const int MID_SPEED = 7000;

enum intakeStates {
    IDLE = 0,
    INTAKING = 1, 
    OUTTAKING = 2, 
    HIGH_SCORE = 3,
    MID_SCORE = 4
};

extern void intakeStateMachine();
extern void setIntake(int speed);
extern void setPitons(bool tri1, bool tri2, bool shared);
extern void intakeTelemetry();