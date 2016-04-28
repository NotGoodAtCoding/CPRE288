/*
 * IR.h
 *
 * Created: 4/13/2016 1:10:06 PM
 *  Author: sakraus
 */ 
#ifndef IR_H
#define IR_H
#include <avr/io.h>

// Convert the index of our table to a cm value
int index_to_cm(int index);

// Plot a value from our recorded value table
int linearRegression(int value);

// Initialize the Infrared sensor
void ADC_init();

// Read from the Infrared sensor
uint16_t ADC_read();

#endif