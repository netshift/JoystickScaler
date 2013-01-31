/*
 * SPAM Programming Team
 * 
 * Author: Nathan Taylor
 * Description: SPAM Joystick Header (JoystickScaler.h)
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
