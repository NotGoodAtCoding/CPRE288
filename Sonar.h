/*
 * Sonar.h
 *
 * Created: 4/13/2016 12:27:16 PM
 *  Author: sakraus
 */ 

// Convert time difference to a distance in cm
float time_to_dist();

// Initialize timer1 for Sonar distance reading
void timer1_init();

// Stop timer 1 for Sonar distance reading
void timer1_stop();

// Send a Sonar pulse for Sonar distance reading
void send_pulse();

// Leverage sonar sensor for a CM distance
float ping_read();
