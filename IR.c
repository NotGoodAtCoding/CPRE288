/*
 * IR.c
 *
 * Created: 4/13/2016 1:09:58 PM
 *  Author: sakraus
 */ 

#ifndef AVR_IO
#define AVR_IO

#include <avr/io.h>

#endif /* AVR_IO */

//IR tables - Num = robot num
int table17[]= {1023, 730, 570, 450, 390, 340, 300, 260, 240, 200, 180, 170, 160, 155, 150, 145};

/// Initialize the Infrared sensor
/**
 * Sets the ADMUX register to settings : 2.56V, Single Ended Input to ADC2
 * Sets the ADCSRA register to settings : Enable ADC Control, Start Conversion, Prescaler to 128
 */
void ADC_init() {
	//Sets ADC Multiplexer 2.56V, Single Ended Input to ADC2
	ADMUX |= 0b11000010;
	//Enable ADC Control, Start Conversion, Set prescalar to 128
	ADCSRA |= 0b11000111;
}

/// Convert the index of our table to a cm value
/**
 * The table of measurements is s pseudo-enum where each value is the measured
 * IR reading at (the index * 5) cm from the sensor. As such, this function returns
 * the cm value of the index of the table.
 * @param index the index of the table to be converted
 * @return the cm measurement represented by the index
 */
int index_to_cm(int index){
	return (index+1) * 5;
}

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
int linearRegression(int value){
	int upper=0, lower=0, i;
	for(i=0; i<16; i++){
		if(table17[i] < value){
			lower = table17[i]; //Lower bound of table value
			upper = table17[i-1]; //Upper bound of table value
			break;
		}
	}
	//Return the cm representation of the upper bound (lower cm value)
	//plus the linear approximation of the position between the values
	return index_to_cm(i-1) + ((upper - lower) / (value - lower));
}

/// Read from the Infrared sensor
/**
 * Sets the ADCSRA register to start the Analog to Digital conversion and returns 
 * the approximated distance of the reading in cm.
 * @return the approximate distance in cm
 */
uint16_t ADC_read() {
	//ADC Start Conversion
	ADCSRA |=      0b11000000;
	while(ADCSRA & 0b01000000){}
	return linearRegression(ADC);
}