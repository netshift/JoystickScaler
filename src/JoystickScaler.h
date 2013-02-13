//
//  JoystickScaler.h
//  SpamProgramming
//
//  Copyright (c) 2013 SPAM Prog. All rights reserved.
//

#ifndef __SpamProgramming__JoystickScaler__
#define __SpamProgramming__JoystickScaler__


#define WITH_TESTING_METHODS
#define USE_RATE_CHANGE_LIMIT

///// Defaults
const double kSensitivityX = 1.5;
const double kSensitivityY = 1.5;
const double kArcTightness = 2.0;   // lower for tighter turns, 1.0 hard turns, 3.0 gradual turns

///// Constants
// To limit jerk when motors are accelerated too quickly. 
const double kMotorDeltaLimit = 0.1;    // 0.1 ... 1.0 - 1.0 for no limit, 0.0 is an error

// filter input 'noise' from minor deviations when near center of an axis.
const double kJoystickCenterTolerance = 0.05;

// pivot zone is when joystick moves along the X axis, y is near 0
const double kPivotZoneWidth = 0.05;



class JoystickScaler {
private:
    double inX, inY;        // Joystick input can be in range -1.0..1.0
    double outX, outY;      // Therefore scaled output is also in range -1.0..1.0

    double motorSpeedTargetL, motorSpeedTargetR;   // Motor speeds must be -1.0..1.0 (0 -> 100%, negative means backwards)
    double motorSpeedOutputL, motorSpeedOutputR;
    
    double sensitivityX, sensitivityY;     // range 0.5...2.0, 1.0 is linear, 2.0 is squared
    
    void ScaleJoystickValues();
    void LimitMotorSpeeds();
    void CalculateMotorSpeeds();
    
public:
    JoystickScaler();
    
    bool SetSensitivity(double senX, double senY, double arc);
    void InputFromJoystick(double x, double y);
    
    double GetX()               { return outX; }
    double GetY()               { return outY; }
    
    double GetMotorLeft();
    double GetMotorRight();

#ifdef WITH_TESTING_METHODS
    // Debugging methods
    void PrintValues();
    void Testing();
#endif // WITH_TESTING_METHODS
};


#endif /* defined(__SpamProgramming__JoystickScaler__) */
