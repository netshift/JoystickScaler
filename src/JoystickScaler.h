/*
 * SPAM Programming Team
 * 
 * Author: Nathan Taylor
 * Description: SPAM Joystick Header (JoystickScaler.h)
 * 
 * Date: 1/30/2013
 */

#ifndef JOYSTICKSCALER_H_
#define JOYSTICKSCALER_H_

class JoystickScaler {
private:
    double inX, inY;        // Joystick input can be in range -1.0..1.0
    double outX, outY;      // Therefore scaled output is also in range -1.0..1.0
    double leftMotorSpeed, rightMotorSpeed;   // Motor speeds must be 0.0..1.0 (0 -> 100%)
    
public:
    JoystickScaler();
    
	void InputFromJoystick(double x, double y);
    
    void ScaleJoystickValues();

    void CalculateMotorSpeeds();
    
    void PrintValues();
    
    void Testing();
};

#endif /* JOYSTICKSCALER_H_ */
