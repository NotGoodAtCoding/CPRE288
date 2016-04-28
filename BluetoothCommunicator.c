/*
 * BluetoothCommunicator.c
 *
 * Created: 4/13/2016 12:29:01 PM
 *  Author: sakraus
 */ 


#include <string.h> /* memset */

#ifndef AVR_IO
#define AVR_IO

#include <avr/io.h>

#endif /* AVR_IO */

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/interrupt.h>

#endif /* INTERRUPT_H */

//USART
#define FOSC 1843200// Clock Speed
#define BAUD 34
#define MYUBRR FOSC/16/BAUD-1

// Initialize serial communication over bluetooth
void serial_init(void) {
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD >> 8);
	UBRR0L = (unsigned char)BAUD;
	
	/* Enable double speed mode */
	UCSR0A = 0b00000010;
	
	/* Set frame format: 8 data bits, 2 stop bits */
	UCSR0C = 0b00001110;
	
	/* Enable receiver and transmitter */
	UCSR0B = 0b00011000;
	
	// UCSR0B |= 0b10000000; // optional: receive interrupt enable bit
}
//Receive data from the serial connection
unsigned char serial_getc() {
	/* Wait for the receive complete flag (RXC) */
	//while ((UCSR0A & 0b10000000) == 0);
	
	while ( !(UCSR0A & 0b10000000) ) {}
	
	/* Reads data from the receive buffer; clears the receive buffer */
	return UDR0;
}

//Transmit Data over the Serial connection
void serial_putc(char data) {

	/* Wait for empty transmit buffer by checking the UDRE bit */
	while ((UCSR0A & 0b00100000) == 0){}
	
	/* Put data into transmit buffer; sends the data */
	UDR0 = data;
}

// Transmit a string of characters over the serial connection
void serial_puts(char* data){
	int i = 0;
	char buf[81];
	memset(buf, ' ', sizeof(char) * 81);
	sprintf(buf, "%s", data);
	for( ; i < 81; i++){
		serial_putc(buf[i]);
	}
}