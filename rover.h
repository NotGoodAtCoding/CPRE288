#ifndef MARS_ROVER_H
#define MARS_ROVER_H

#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

#include <math.h>

#include <stdio.h>
#include <avr/io.h>

#include "open_interface.h"

// LCD Display components, misc utilities
#include "lcd.h"
#include "util.h"

// Custom components
#include "IR.h"
#include "Sonar.h"
#include "BluetoothCommunicator.h"
#include "Servo.h"
#include "Movement.h"
#include "music.h"
#include "map.h"

//Misc Constants
#define PI 3.14159265

//Movement constants
#define MOVE 1
#define TURN 2
#define NONE -1
#define WIN 3

// Boundry detection
#define NO_BOUNDRY 0
#define WHITE_BOUNDRY 1
#define FINISH_BOUNDRY 2
#define WHITE_BOUNDARY_LOWER 700
#define WHITE_BOUNDARY_UPPER 950

/// Initialize all necessary components in the oi_t struct
/**
 * Aggregation of calls for initializers of sensors used by the program
 * @return the fully initialized oi_t struct
 */ 
oi_t * init();

/// Do a rapid forward scan from approx 40 - 140 to prevent collisions
/**
 * Performs a scan of the immediate area in front of the iRobot and
 * returns the angle of incidence of any object detected.
 * @return the angle of detection of any objects, 0 otherwise
 */ 
int rapidForwardScan();

/// Detect tape boundary with light sensor
/**
 * Uses the passed in sensor data to detect if a boundary has been reached.
 * If a white tape boundary is detected, reacts accordingly by turning left or right to 
 * avoid the boundary. If the finish color is detected, the win procedure is started.
 * @param sensor_data the oi_t struct representing the sensor data of the robot
 * @return 1 if boundary detected, 2 if finish detected, 0 otherwise
 */
int detectColoredBoundry(oi_t * sensor_data);

/// Detects a BLACK finish color, WHITE boundary color, and grey passable floor color
/**
 * Custom detection method to account for black finish
 * @param reading an int representing the light sensor reading
 * @param base the base reading of the light sensors detecting normal floor tiles
 * @return 1 if boundary detected, 2 if finish detected, 0 otherwise
 */
int determineBoundryType(int reading, int base);

/// Move forward at a speed allwong for object and cliff detection
/**
 * Moves forward and performs periodic checks for Objects, Boundaries, and Crater
 * course features.
 * @param cm centimeters to move forward at a reasonable pace
 * @param sensorData a pointer to the sensor data struct
 * @return FINISH_BOUNDRY if finish found, 0 otherwise
 */
int moveCautiously(int cm, oi_t * sensorData);

/// Prints string to LCD Display and Serial connection
/**
 * Makes subsequent calls to the string printing methods associated
 * with the LCD Display and the Serial Connection
 * @param *s, char pointer to the string to be printed, must be no longer than 80 chars
 */
void print(char* s);

/// Procedure for finding the finish
/**
 * On finding the finish, 'pop a sick wheelie, pop in the sick beats, then donuts for days'
 * @param sensorData, a pointer to the oi_t struct
 */
void winProcedure(oi_t * sensorData);

/// Main method to run the automation loop 
/**
 * Moves cautiously, avoiding obstacles until the finish is reached, then performs the 
 * win procedure.
 */
int main(void);



#endif