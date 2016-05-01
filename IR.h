/*
 * IR.h
 *
 * Created: 4/13/2016 1:10:06 PM
 *  Author: sakraus
 */ 
#ifndef IR_H
#define IR_H
#include <avr/io.h>

/// Initialize the Infrared sensor
/**
 * Sets the ADMUX register to settings : 2.56V, Single Ended Input to ADC2
 * Sets the ADCSRA register to settings : Enable ADC Control, Start Conversion, Prescaler to 128
 */
void ADC_init();

/// Convert the index of our table to a cm value
/**
 * The table of measurements is s pseudo-enum where each value is the measured
 * IR reading at (the index * 5) cm from the sensor. As such, this function returns
 * the cm value of the index of the table.
 * @param index the index of the table to be converted
 * @return the cm measurement represented by the index
 */
int index_to_cm(int index);

/// Plot a value from our recorded value table
/**
 * Use a linear approximative algoritm to quickly calculate the distance in 
 * cm that a reading from the IR sensor corresponds to. This is done by 
 * referencing a table of known readings taken every 5 cm and calculating
 * the upper and lower bound of the cm distances the reading corresponds to.
 * Then, a linear approximation is made given the ratio of the reading and the
 * IR values associated with the upper and lower bounds.
 * @param value the IR reading to be approximated
 * @return the approximate distance in cm that value corresponds to 
 */
int linearRegression(int value);


/// Read from the Infrared sensor
/**
 * Sets the ADCSRA register to start the Analog to Digital conversion and returns 
 * the approximated distance of the reading in cm.
 * @return the approximate distance in cm
 */
uint16_t ADC_read();

#endif