
/*
 * Movement.c
 *
 * Created: 4/13/2016 1:49:30 PM
 *  Author: sakraus
 */ 

//TODO: Include guards
#include <stdint.h>
#include <stdlib.h>

#ifndef OPEN_H
#define OPEN_H

#include "open_interface.h"

#endif /* OPEN_H */

#ifndef STDIO
#define STDIO

#include <stdio.h>

#endif /* STDIO */

#ifndef MATH_H
#define MATH_H

#include <math.h>

#endif /* MATH_H */

#ifndef LCD_H
#define LCD_H

#include "lcd.h"

#endif /* LCD_H */

#ifndef UTIL_H
#define UTIL_H

#include "util.h"

#endif /* UTIL_H */

#ifndef AVR_IO
#define AVR_IO

#include <avr/io.h>

#endif /* AVR_IO */

// Move with right, left wheel speed for distance - TODO: Distance is in CM?
void move(int right, int left, int distance, oi_t *sensor_data){
	int sum = 0;
	oi_set_wheels(right, left); // move forward; full speed
	
	distance = abs(distance);
	
	while (sum < distance) {
		oi_update(sensor_data);

		if ( right > 0 && left > 0 ) {
			// Only reset the wheels if we stopped
			if ( detectCollision(sensor_data) ) {
				oi_set_wheels(right, left);
			}
		}

		sum += abs(sensor_data->distance);
	}
	
	oi_set_wheels(0,0);
}

// Turn turnDegree
void turn(int turnDegrees, oi_t * sensor_data){
	int currentDegrees = 0;
	
	oi_set_wheels((turnDegrees / 3) * 2,(-1 * turnDegrees / 3) * 2);
	
	turnDegrees = abs(turnDegrees);

	while ( currentDegrees < turnDegrees ) {
		wait_ms(40);
		oi_update(sensor_data);
		currentDegrees += abs(sensor_data->angle);
	}
	
	oi_set_wheels(0,0); //Stop
}

// Detect and react to collision - TODO: Different collision detection subroutine?
int detectCollision(oi_t * sensor_data){
	int left_hit = sensor_data->bumper_left;
	int right_hit = sensor_data->bumper_right;
	
	if ( left_hit || right_hit ) {
		print("Collision!");
		move(-100, -100, 150, sensor_data);
		return 1;
	} else {
		return 0;
	}
	
	return 0;
}