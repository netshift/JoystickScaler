/* SPAM Programming Team
 *
 * Author: Nathan Taylor
 * Description: SPAM Joystick Code
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

#include "JoystickScaler.h"

int main()
{	
    JoystickScaler myScaler;
    myScaler.Testing();
    return 0;
}

