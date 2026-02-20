#pragma once
#include "lemlib/api.hpp"
#include "pros/distance.hpp"

// positive # indicates sensor is closer to the wall in inches
const double BACK_SENSOR_OFFSET = 7.0;
const double LEFT_SENSOR_OFFSET = 7.0;

const double FIELD_HALF = 72.0;

inline pros::Distance backSensor(10);
inline pros::Distance leftSensor(1);

extern lemlib::Chassis chassis; // reference the chassis so we can use lem functions