/* SPAM Programming Team
 *
 * Author: Nathan Taylor
 * Description: SPAM Joystick Code (JoystickScaler.cpp)
 *
 * Problem: We have a joystick that controls the robot
 * The values need to be modified so the driver is given more control sensitivity
 *
 * Joystick outputs an x and y value that we take as inputs
 *      Scale the input values
 *      Then calculate the proper left and right motor speeds
 *
 * Date: 1/24/2013
 */
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include "JoystickScaler.h"

using namespace std;

JoystickScaler::JoystickScaler() {
	// initialize all member variables
	inX = 0.0;
	inY = 0.0;
	outX = 0.0;
	outY = 0.0;
	leftMotorSpeed = 0.0;
	rightMotorSpeed = 0.0;
}
    
void JoystickScaler::InputFromJoystick(double x, double y) {
	
	// we should not need to limit the inputs if the joystick is working properly
	// but it is good practice
	// limit the input values to -1.0..1.0
	if (x < -1.0)   x = -1.0;
	if (x > 1.0)    x = 1.0;
	if (y < -1.0)   y = -1.0;
	if (y > 1.0)    y = 1.0;
	
	
	// On the x-axis, our joystick outputs positive values for right and
	// negative for left. Just the way we like it.
	
	inX = x;
	
	
	// On the y-axis, our joystick outputs negative values when pushed forward.
	// We would prefer pushing the joystick forward to make the robot move forward,
	// so we flip the sign of the y-axis.
	
	inY = -y;       // this could also be  inY = (y * -1.0)
}

void JoystickScaler::ScaleJoystickValues() {
	// setting the sensitivity
	// (k is a C naming convention)
	const double kSensitivityX = 1.5;
	const double kSensitivityY = 1.5;
	
	// true for positive, false for negative
	bool bPositiveX = (inX >= 0);
	bool bPositiveY = (inY >= 0);
	
	// get the absolute value for X and Y
	double absoluteX = fabs(inX);
	double absoluteY = fabs(inY);
	
	// the pow function that applies the scaling
	outX = pow(absoluteX, kSensitivityX);
	outY = pow(absoluteY, kSensitivityY); 
	
	if (bPositiveX == false)
		outX = -outX;
	
	if (bPositiveY == false)
		outY = -outY;        
}

void JoystickScaler::CalculateMotorSpeeds() {
	//if (inY >= 0) {
		//going forward
	//if X is positive Motor R decreases
	// motor R = 1.0 - outX
	// technically wouldn't work because when = 0 everything stops
	// scale outX somewhere 1.0 - (outX / 2)
	leftMotorSpeed = outY;
	rightMotorSpeed = outY - (outX / 2);
	//} else {
		//going backwards
	//}
		   
	//leftMotorSpeed = 0.30;
	//rightMotorSpeed = 0.40;
   
}

void JoystickScaler::PrintValues() {
	// the printf function allows us to make a template for displaying our variables
	// %0.2f  means to display 2 decimal places.
	// \t is a tab character, and \n is a new line
	printf("input %0.2f  %0.2f \t output %0.2f  %0.2f \t", inX, inY, outX, outY);
	printf("motor left %0.2f \t right %0.2f\n", leftMotorSpeed, rightMotorSpeed);
}


void JoystickScaler::Testing() {
	// We don't have a joystick hooked up to the computer so simulate one.
	// Input various x,y values into our class, and watch the results.
	double simJoyX = 0.0;
	double simJoyY = 0.0;
	
	// loop through the possible values for the y-axis,
	// start with the stick forward (-1.0) and then gradually pulling all the way back to 1.0
	
	printf("Testing X and Y Axis:\n");
   
	for (simJoyY = -1.0; simJoyY <= 1.0; simJoyY += 0.1)
	{
		simJoyX = -(simJoyY) + 0.30;
		InputFromJoystick(simJoyX, simJoyY);
		ScaleJoystickValues();
		CalculateMotorSpeeds();
		PrintValues();
	}
}

