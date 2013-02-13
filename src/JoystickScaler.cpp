//
//  JoystickScaler.cpp
//  SpamProgramming
//
//  Copyright (c) 2013 SPAM Prog. All rights reserved.
//

#include "JoystickScaler.h"

//#include <iostream>
//#include <cstdlib>
#include <math.h>
#include <stdio.h>



JoystickScaler::JoystickScaler() {
    // initialize all member variables
    inX = 0.0;
    inY = 0.0;
    outX = 0.0;
    outY = 0.0;
    
    motorSpeedTargetL = 0.0;
    motorSpeedTargetR = 0.0;
    motorSpeedOutputL = 0.0;
    motorSpeedOutputR = 0.0;
    
    SetSensitivity(kSensitivityX, kSensitivityY, kArcTightness);
}

bool JoystickScaler::SetSensitivity(double senX, double senY, double arc)
{
    //TODO: check limits, return false if out of limits
    
    sensitivityX = kSensitivityX;
    sensitivityY = kSensitivityY;
    
    return true;
}

void JoystickScaler::InputFromJoystick(double x, double y) {
    
    // we should not need to limit the inputs if the joystick is working properly
    // limit the input values to -1.0..1.0
    if (x < -1.0)   x = -1.0;
    if (x > 1.0)    x = 1.0;
    if (y < -1.0)   y = -1.0;
    if (y > 1.0)    y = 1.0;
 
    // filter 'noise' from minor deviations when near center of an axis.
    if (fabs(x) < kJoystickCenterTolerance)    x = 0;
    if (fabs(y) < kJoystickCenterTolerance)    y = 0;

    
    // On the x-axis, our joystick outputs positive values for right and
    // negative for left. Just the way we like it.
    
    inX = x;
    
    
    // On the y-axis, our joystick outputs negative values when pushed forward.
    // We would prefer pushing the joystick forward to make the robot move forward,
    // so we flip the sign of the y-axis.
    
    inY = -y;       // this could also be  inY = (y * -1.0)
    
    ScaleJoystickValues();
}

void JoystickScaler::ScaleJoystickValues() {
    
    // true for positive, false for negative
    bool bPositiveX = (inX >= 0);
    bool bPositiveY = (inY >= 0);
    
    // get the absolute value for X and Y
    double absoluteX = fabs(inX);
    double absoluteY = fabs(inY);
    
    // the pow function that applies the scaling
    outX = pow(absoluteX, sensitivityX);
    outY = pow(absoluteY, sensitivityY);
    
    if (bPositiveX == false)
        outX = -outX;
    
    if (bPositiveY == false)
        outY = -outY;
    
    CalculateMotorSpeeds();
}

void JoystickScaler::CalculateMotorSpeeds() {
    
    
    if (fabs(outY) < kPivotZoneWidth && fabs(outX) > kPivotZoneWidth)   // hardly moving forward or back with a turning component
    {
        // pivot by putting opposite power to the motors.
        motorSpeedTargetL = outX  + outY;
        motorSpeedTargetR = -outX + outY;
        
        LimitMotorSpeeds();
        return;
    }
    
    // The idea is to attenuate the speed of the motor on the inside of the turn
    // causing the robot to travel along a constant arc (forward or reverse)
    // as X increases the arc will tighten
    
    motorSpeedTargetL = outY;
    motorSpeedTargetR = outY;
    
    double attenuatePercent = 1.0 - fabs(outX / kArcTightness);   // adjust the divisor - smaller for tighter turns.
    if (outX < 0)
        motorSpeedTargetL *= attenuatePercent;
    else if (outX > 0)
        motorSpeedTargetR *= attenuatePercent;
    
    LimitMotorSpeeds();
}

// A quick and dirty application of a rate change limit
// assumes the motor values are queried periodically and will change
// no more than kMotorDeltaLimit between subsequent calls
double JoystickScaler::GetMotorLeft()
{    
    return motorSpeedOutputL;
}
double JoystickScaler::GetMotorRight()
{
    return motorSpeedOutputR;
}

void JoystickScaler::LimitMotorSpeeds() {

#ifdef USE_RATE_CHANGE_LIMIT
    double deltaL = motorSpeedTargetL - motorSpeedOutputL;
    if (deltaL > kMotorDeltaLimit)
        deltaL = kMotorDeltaLimit;
    else if (deltaL < -kMotorDeltaLimit)
        deltaL = -kMotorDeltaLimit;
    motorSpeedOutputL += deltaL;

    double deltaR = motorSpeedTargetR - motorSpeedOutputR;
    if (deltaR > kMotorDeltaLimit)
        deltaR = kMotorDeltaLimit;
    else if (deltaR < -kMotorDeltaLimit)
        deltaR = -kMotorDeltaLimit;
    motorSpeedOutputR += deltaR;
#else

    motorSpeedOutputL = motorSpeedTargetL;
    motorSpeedOutputR = motorSpeedTargetR;

#endif  // USE_RATE_CHANGE_LIMIT
    
    // enforce the limits of -1.0...1.0 for the motor speeds
    if (motorSpeedOutputL < -1.0)   motorSpeedOutputL = -1.0;
    if (motorSpeedOutputL > 1.0)    motorSpeedOutputL = 1.0;
    if (motorSpeedOutputR < -1.0)  motorSpeedOutputR = -1.0;
    if (motorSpeedOutputR > 1.0)   motorSpeedOutputR = 1.0;
}

#ifdef WITH_TESTING_METHODS

void JoystickScaler::PrintValues() {
    // the printf function allows us to make a template for displaying our variables
    // %0.2f  means to display 2 decimal places.
    // \t is a tab character, and \n is a new line
    //printf("in %0.2f %0.2f", inX, inY);
    printf("\t out %0.2f %0.2f", GetX(), GetY());
    printf("\t motor L: %0.2f \tR: %0.2f\n", GetMotorLeft(), GetMotorRight());
}


void JoystickScaler::Testing() {
    // We don't have a joystick hooked up to the computer so simulate one.
    // Input various x,y values into our class, and watch the results.
    double simJoyX = 0.0;
    double simJoyY = 0.0;
    
    // loop through the possible values for the y-axis,
    // start with the stick forward (-1.0) and then gradually pulling all the way back to 1.0
    
    printf("\n== Testing Y Axis forward to back, X neutral:\n");
    for (simJoyY = -1.0; simJoyY <= 1.0; simJoyY += 0.2)
    {
        simJoyX = 0.0;
        InputFromJoystick(simJoyX, simJoyY);
        PrintValues();
    }
    
    printf("\n== Testing X Axis left to right, Y 100%% Forward:\n");
    for (simJoyX = -1.0; simJoyX <= 1.0; simJoyX += 0.2)
    {
        simJoyY = -1.0;
        InputFromJoystick(simJoyX, simJoyY);
        PrintValues();
    }
    
    printf("\n== Testing X Axis left to right, Y 08%% Reverse:\n");
    for (simJoyX = -1.0; simJoyX <= 1.0; simJoyX += 0.2)
    {
        simJoyY = 0.08;
        InputFromJoystick(simJoyX, simJoyY);
        PrintValues();
    }
    
    printf("\n== Testing Rate change limit\n");
    for (long x = 0; x < 20; x++)
    {
        InputFromJoystick(1, 1);
        PrintValues();
    }
    
}
#endif // WITH_TESTING_METHODS
