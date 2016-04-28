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

// Convert the index of our table to a cm value
int index_to_cm(int index){
	return (index+1) * 5;
}

// Plot a value from our recorded value table
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

// Initialize the Infrared sensor
void ADC_init() {
	//Sets ADC Multiplexer 2.56V, Single Ended Input to ADC2
	ADMUX |= 0b11000010;
	//Enable ADC Control, Start Conversion, Set prescalar to 128
	ADCSRA |= 0b11000111;
}

// Read from the Infrared sensor
uint16_t ADC_read() {
	//ADC Start Conversion
	ADCSRA |=      0b11000000;
	while(ADCSRA & 0b01000000){}
	return linearRegression(ADC);
}