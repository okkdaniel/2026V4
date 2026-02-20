#include "lemlib/api.hpp"
#include "pros/distance.hpp"
#include "liblvgl/llemu.hpp"

// positive # indicates closer to the wall, past the center of the robot in that plane, in inches
const double BACK_SENSOR_OFFSET = 7.0;
const double LEFT_SENSOR_OFFSET = 7.0;

// change ts later if needed in inches
const double FIELD_HALF = 72.0;

pros::Distance backSensor(10);
pros::Distance leftSensor(1);

extern lemlib::Chassis chassis;

void localize(bool useBack = true, bool useLeft = true, double maxDist = 60.0)
{
    lemlib::Pose currentPose = chassis.getPose();
    double headingRad = currentPose.theta * M_PI/180.0; // conversion to radians

    double newX = currentPose.x;
    double newY = currentPose.y;

    if(useBack)
    {
        double rawBack = backSensor.get()/25.4; // convert from mm to inches

        if(rawBack > 0 && rawBack < maxDist)
        {
            //correct for small heading errors
            double backAngle = headingRad * M_PI;

            double sensorX = currentPose.x + BACK_SENSOR_OFFSET * sin(headingRad - M_PI);
            double sensorY = currentPose.y + BACK_SENSOR_OFFSET * cos(headingRad - M_PI);

            double dx = sin(backAngle);
            double dy = cos(backAngle);

            if(fabs(dy)>fabs(dx))
            {
                double wallY = (dy<0) ? -FIELD_HALF:FIELD_HALF;
                newY = wallY - rawBack * dy - (sensorY - currentPose.y);
            }
            else 
            {
                double wallX = (dx<0) ? -FIELD_HALF:FIELD_HALF;
                newX = wallX - rawBack * dx - (sensorX - currentPose.x);
            }
        }
    }

    if(useLeft)
    {
        double rawLeft = leftSensor.get()/25.4; // convert from mm to inches

        if(rawLeft > 0 && rawLeft < maxDist)
        {
            //correct for small heading errors
            double leftAngle = headingRad - M_PI/2.0;

            double sensorX = currentPose.x + LEFT_SENSOR_OFFSET * sin(leftAngle);
            double sensorY = currentPose.y + LEFT_SENSOR_OFFSET * cos(leftAngle);

            double dx = sin(leftAngle);
            double dy = cos(leftAngle);

            if(fabs(dy)>fabs(dx))
            {
                double wallY = (dy<0) ? -FIELD_HALF:FIELD_HALF;
                newY = wallY - rawLeft * dy - (sensorY - currentPose.y);
            }
            else 
            {
                double wallX = (dx<0) ? -FIELD_HALF:FIELD_HALF;
                newX = wallX - rawLeft * dx - (sensorX - currentPose.x);
            }
        }
    }

    chassis.setPose(newX, newY, currentPose.theta);
}