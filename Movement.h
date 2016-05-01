
/*
 * Movement.h
 *
 * Created: 4/13/2016 1:49:36 PM
 *  Author: sakraus
 */ 

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "open_interface.h"
#include "rover.h"


/// Move with right, left wheel speed for distance 
/**
 * Energizes the wheels of the iRobot for some distance, then stops the wheels.
 * @param right the speed of the right wheel
 * @param left the speed of the left wheel
 * @param distance the distance in mm to travel
 * @param *sensor_data a pointer to the sensor data struct
 */
int move(int right, int left, int distance, oi_t *sensor_data);

/// Turn the robot a number of degrees
/**
 * Calls the set_wheels function to turn the iRobot. turnDegrees should be 
 * negative for a clockwise rotation, positive for a CCW rotation.
 * @param turnDegrees the number or degrees to turn.
 * @param *sensor_data a pointer to the sensor data struct
 */
void turn(int turnDegrees, oi_t * sensor_data);

/// Detect and react to collision
/**
 * Default behavior for object detection. 
 * @param *sensor_data a pointer to the sensor data struct
 */
int detectCollision(oi_t * sensor_data);

#endif