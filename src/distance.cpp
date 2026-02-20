#include "distance.hpp"

void localize(bool useBack = true, bool useLeft = true, double maxDist = 60.0)
{
    lemlib::Pose pose = chassis.getPose();

    double headingRad = pose.theta * M_PI/180.0; // convert to radians

    double correctedX = pose.x;
    double correctedY = pose.y;

    const double ALIGN_THRESHOLD = 0.9; // is the robot parallel to wall?
    const double BLEND = 0.4; // how much to correct the pose by, make it closer to one if we trust the sensors more than lem odom

    auto applyCorrection = [&](double sensorDist,
                                         double sensorAngle,
                                         double sensorOffset){
        
        if(sensorDist <= 0 || sensorDist >= maxDist) return;
        
        double dx = sin(sensorAngle);
        double dy = cos(sensorAngle);

        double sensorX = pose.x + sensorOffset * sin(sensorAngle);
        double sensorY = pose.y + sensorOffset * cos(sensorAngle);

        if (fabs(dy) > ALIGN_THRESHOLD)
        {
            double wallY = (dy < 0) ? -FIELD_HALF : FIELD_HALF;

            double calculatedRobotY =
                wallY - sensorDist * dy - (sensorY - pose.y);

            correctedY = pose.y + (calculatedRobotY - pose.y) * BLEND; // corrects y pose and blends with current
        }

        else if(fabs(dx) > ALIGN_THRESHOLD)
        {
            double wallX = (dx < 0)  ? -FIELD_HALF : FIELD_HALF;

            double calculatedRobotX =
                wallX - sensorDist * dx - (sensorX - pose.x);

            correctedX = pose.x + (calculatedRobotX - pose.x) * BLEND; // corrects x pose and blends with current
        }
    };

    if(useBack)
    {
        double backDist = backSensor.get() / 25.4; // convert from mm to inches
        double backAngle = headingRad + M_PI; // faces backwards so add 180 degrees
        applyCorrection(backDist, backAngle, BACK_SENSOR_OFFSET);
    }

    if(useLeft)
    {
        double leftDist = leftSensor.get() / 25.4; // convert from mm to inches
        double leftAngle = headingRad - M_PI / 2.0; // faces left so subtract 90 degrees
        applyCorrection(leftDist, leftAngle, LEFT_SENSOR_OFFSET);
    }

    chassis.setPose(correctedX, correctedY, pose.theta);
}