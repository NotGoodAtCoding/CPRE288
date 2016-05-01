/*
 * Servo.h
 *
 * Created: 4/13/2016 1:13:11 PM
 *  Author: sakraus
 */ 

/// Initialize timer for Servo sensor
/**
 * Sets the TCCR3A register to OCRA for top and compare on B
 * Sets the TCCR3B register to OCRA for top and prescalar 8
 * Sets the OCR3A register to TOP 
 * Sets the OCR3B register to the pulse_width
 * Initializes the DDRE port for servo function
 */
void timer3_init();

/// Move servo to angle degree
/**
 * Sets the pulse_width and assigns it to the OCR3B register
 * @param degree an unsigned int between 0 and 180 to set the servo to
 */
void move_servo(unsigned degree);

/// Calculate arc distance given degree difference and distance
/**
 * Given a degree span and an instance distance, calculates the arc length
 * @param degrees the degree span
 * @param dist the distance to the artifact
 * @return the arc length of the area covered by the given parameters
 */
float arc_distance(float degrees, float dist);
