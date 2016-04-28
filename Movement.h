
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


// Move with right, left wheel speed for distance
void move(int right, int left, int distance, oi_t *sensor_data);

// Turn turnDegree
void turn(int turnDegrees, oi_t * sensor_data);

// Detect and react to collision
int detectCollision(oi_t * sensor_data);

#endif