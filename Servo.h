/*
 * Servo.h
 *
 * Created: 4/13/2016 1:13:11 PM
 *  Author: sakraus
 */ 

// Initialize timer for Serve sensor
void timer3_init();

// Move servo to angle degree
void move_servo(unsigned degree);

//Calculate arc distance given degree difference and distance
float arc_distance(float degrees, float dist);
