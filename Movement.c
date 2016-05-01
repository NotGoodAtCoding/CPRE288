
/*
 * Movement.c
 *
 * Created: 4/13/2016 1:49:30 PM
 *  Author: sakraus
 */ 

#include <stdint.h>
#include <stdlib.h>

#include "open_interface.h"
#include <stdio.h>
#include <math.h>
#include "lcd.h"
#include "util.h"
#include <avr/io.h>

/// Move with right, left wheel speed for distance 
/**
 * Energizes the wheels of the iRobot for some distance, then stops the wheels.
 * @param right the speed of the right wheel
 * @param left the speed of the left wheel
 * @param distance the distance in mm to travel
 * @param *sensor_data a pointer to the sensor data struct
 */
int move(int right, int left, int distance, oi_t *sensor_data){
	int sum = 0;
	oi_set_wheels(right - 8, left); // move forward; full speed
	
	distance = abs(distance);
	
	while (sum < distance) {
		oi_update(sensor_data);

		if ( right > 0 && left > 0 ) {
			// Only reset the wheels if we stopped
			if ( detectCollision(sensor_data) ) {
				return 1;
			}
		}

		sum += abs(sensor_data->distance);
	}
	
	oi_set_wheels(0,0);
	
	return 0;
}

/// Turn the robot a number of degrees
/**
 * Calls the set_wheels function to turn the iRobot. turnDegrees should be 
 * negative for a clockwise rotation, positive for a CCW rotation.
 * @param turnDegrees the number or degrees to turn.
 * @param *sensor_data a pointer to the sensor data struct
 */
void turn(int turnDegrees, oi_t * sensor_data){
	int currentDegrees = 0;
	int magnitude = turnDegrees > 0 ? 1 : -1;
	
	oi_set_wheels(magnitude * 100, -1 * magnitude * 100);
	
	turnDegrees = abs(turnDegrees);

	while ( currentDegrees < turnDegrees ) {
		wait_ms(40);
		oi_update(sensor_data);
		currentDegrees += abs(sensor_data->angle);
	}
	
	oi_set_wheels(0,0); //Stop
}

/// Detect and react to collision
/**
 * Default behavior for object detection. 
 * @param *sensor_data a pointer to the sensor data struct
 */
int detectCollision(oi_t * sensor_data){
	int left_hit = sensor_data->bumper_left;
	int right_hit = sensor_data->bumper_right;
	
	if ( left_hit || right_hit ) {
		print("Collision!");
		move(-100, -100, 50, sensor_data);
		turn(60, sensor_data);
		return 1;
	} else {
		return 0;
	}
	
	return 0;
}