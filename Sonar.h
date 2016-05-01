/*
 * Sonar.h
 *
 * Created: 4/13/2016 12:27:16 PM
 *  Author: sakraus
 */ 

/// Convert time difference to a distance in cm
/**
 * Converts the difference between the time records of the rising and falling
 * edges of the Sonar sensor to the approximate distance to the reflecting surface
 * in cm
 * @return the distance to the reflecting surface in cm
 */
float time_to_dist();

/// Initialize timer1 for Sonar distance reading
/**
 * Set the TCCR1A register to allow for input capture
 * Set the TCCR1C register to Set output compare a,b,c to 0
 * Set the TIFR register to enable the interrupts
 * Set the TIMSK register to Enable ic interrupt, overflow interrupt
 * Set the TCCR1B register to Noise cancel, select rising edge, choose 1024 prescalar
 */
void timer1_init();

/// Stop timer 1 for Sonar distance reading
/**
 * Set the TIMSK register to disable IC Interrupt for Timer 1
 */
void timer1_stop();

/// Send a Sonar pulse for Sonar distance reading
/**
 * Set DDRD and PORTD to send a pulse, then wait 1ms and capture input
 */
void send_pulse();

/// Leverage sonar sensor for a CM distance
/**
 * Stop the timer, then send a pulse and reset the timer and wait for a response.
 * When a response is received, return the calculated distance 
 * @ return a float distance to the reflective surface
 */
float ping_read();
