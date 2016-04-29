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

oi_t * init();
int rapidForwardScan();
int detectColoredBoundry(oi_t * sensor_data);
int determineBoundryType(int reading, int base);
int moveCautiously(int cm, oi_t * sensorData);
void print(char* s);
void winProcedure(oi_t * sensorData);
int main(void);



#endif