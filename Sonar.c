/*
 * Sonar.c
 *
 * Functions of or relating to SONAR distance sensor
 *
 * Created: 4/13/2016 12:27:00 PM
 *  Author: sakraus
 */ 



#ifndef AVR_IO
#define AVR_IO

#include <avr/io.h>

#endif /* AVR_IO */


#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/interrupt.h>

#endif /* INTERRUPT_H */

#ifndef STDIO
#define STDIO

#include <stdio.h>

#endif /* STDIO */


#ifndef UTIL_H
#define UTIL_H

#include "util.h"

#endif /* UTIL_H */


// State for ping distance read
volatile enum {RISING, FALLING, DONE} state;
volatile unsigned rising_time = 0;
volatile unsigned falling_time = 0;

/// ISR for edge processing
ISR(TIMER1_CAPT_vect){
	if ( state == RISING ) {
		rising_time = ICR1;
		TCCR1B &= 0b10111111; //Wait for falling edge
		state = FALLING;
		} else if ( state == FALLING) {
		falling_time = ICR1;
		state = DONE;
	}
}

/// Convert time difference to a distance in cm
/**
 * Converts the difference between the time records of the rising and falling
 * edges of the Sonar sensor to the approximate distance to the reflecting surface
 * in cm
 * @return the distance to the reflecting surface in cm
 */
float time_to_dist(){
	float timeDifference = (falling_time - rising_time) / 2; //one way distance
	timeDifference = timeDifference / 15625; //Clock cycles per second with prescalar
	timeDifference = timeDifference * 340; //Seconds difference * 340 m/s (speed of sound) --> meters
	timeDifference = timeDifference * 100; //meters --> cm
	return timeDifference;
}

/// Initialize timer1 for Sonar distance reading
/**
 * Set the TCCR1A register to allow for input capture
 * Set the TCCR1C register to Set output compare a,b,c to 0
 * Set the TIFR register to enable the interrupts
 * Set the TIMSK register to Enable ic interrupt, overflow interrupt
 * Set the TCCR1B register to Noise cancel, select rising edge, choose 1024 prescalar
 */
void timer1_init(){
	TCCR1A =  0b00000000; //Set to input capture
	TCCR1C &= 0b00000000; //Sets output compare a,b,c to 0
	TIFR |=  0b00100000;
	TIMSK |=  0b00100000; //Enable ic interrupt, overflow interrupt
	TCCR1B = 0b11000101; //Noise cancel, select rising edge, choose 1024 prescalar
}

/// Stop timer 1 for Sonar distance reading
/**
 * Set the TIMSK register to disable IC Interrupt for Timer 1
 */
void timer1_stop() {
	TIMSK &= 0b11011011; //Disabling ic interrupt for Timer1
}

/// Send a Sonar pulse for Sonar distance reading
/**
 * Set DDRD and PORTD to send a pulse, then wait 1ms and capture input
 */
void send_pulse(){
	DDRD |= 0x10;  //Enable output to SONAR
	PORTD |= 0x10; //Output to pin 4 (SONAR) - i.e, pulse
	wait_ms(1);    //Wait 1 ms
	PORTD &= 0xEF; //Set pin 4 to 0 (Clear value)
	DDRD &= 0xEF;  //Enable input from pin 4 (SONAR)
}

/// Leverage sonar sensor for a CM distance
/**
 * Stop the timer, then send a pulse and reset the timer and wait for a response.
 * When a response is received, return the calculated distance 
 * @ return a float distance to the reflective surface
 */
float ping_read(){
	state = RISING;
	
	//Disable timer 1, clear flag
	timer1_stop();
	
	//Send sonar pulse and prepare for input
	send_pulse();
	
	//Enable interrupt
	timer1_init();
	
	while ( state != DONE) {} //Busy wait for reading
	
	//Return difference
	return time_to_dist();
}