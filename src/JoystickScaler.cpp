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
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	// if X is positive Motor R decreases to cause a turn to the right.
	
	// motor R = 1.0 - outX
	// technically wouldn't work because when = 0 everything stops

	// The idea is to attenuate the speed of the motor on the inside of the turn
	
	// This algorithm works well if Y is 1.0, but it scretches to a halt if Y = 0.5
	leftMotorSpeed = outY + (outX / 2);
	rightMotorSpeed = outY - (outX / 2);
	
	// enforce the limits of -1.0...1.0 for the motor speeds
	if (leftMotorSpeed < -1.0)   leftMotorSpeed = -1.0;
	if (leftMotorSpeed > 1.0)    leftMotorSpeed = 1.0;
	if (rightMotorSpeed < -1.0)  rightMotorSpeed = -1.0;
	if (rightMotorSpeed > 1.0)   rightMotorSpeed = 1.0;
   
}

void JoystickScaler::PrintValues() {
	// the printf function allows us to make a template for displaying our variables
	// %0.2f  means to display 2 decimal places.
	// \t is a tab character, and \n is a new line
	printf("in %0.2f %0.2f", inX, inY);
	//printf("\t out %0.2f %0.2f", outX, outY);
	printf("\t motor L: %0.2f \tR: %0.2f\n", leftMotorSpeed, rightMotorSpeed);
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
		ScaleJoystickValues();
		CalculateMotorSpeeds();
		PrintValues();
	}
	
	printf("\n== Testing X Axis left to right, Y 100%% Forward:\n");
	for (simJoyX = -1.0; simJoyX <= 1.0; simJoyX += 0.2)
	{
		simJoyY = -1.0;
		InputFromJoystick(simJoyX, simJoyY);
		ScaleJoystickValues();
		CalculateMotorSpeeds();
		PrintValues();
	}
	
	printf("\n== Testing X Axis left to right, Y 50%% Forward:\n");
	for (simJoyX = -1.0; simJoyX <= 1.0; simJoyX += 0.2)
	{
		simJoyY = -0.5;
		InputFromJoystick(simJoyX, simJoyY);
		ScaleJoystickValues();
		CalculateMotorSpeeds();
		PrintValues();
	}
}

