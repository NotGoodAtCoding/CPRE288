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

/// Initialize serial communication over bluetooth
/**
 * Set the UBBR0H and UBRR0L registers to the BAUD rate 34. 
 * Set the UCSR0A register to enable double speed mode.
 * Set the UCSR0C register to set the frame format: 8 data bits, 2 stop bits
 * Set the UCSR0B register to enable receiver and transmitter.
 */
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
}
/// Receive data from the serial connection
/**
 * Waits for a recieve complete flag in the UCSR0A register, then
 * returns the data in the UDR0 register and clears it.
 * @return the char representation of the data in the UDR0 register
 */
unsigned char serial_getc() {
	/* Wait for the receive complete flag (RXC) */
	while ( !(UCSR0A & 0b10000000) ) {}
	
	/* Reads data from the receive buffer; clears the receive buffer */
	return UDR0;
}

/// Transmit Data over the Serial connection
/**
 * Wait for empty transmit buffer by checking the UDRE bit 
 * puts the data in the UDR0 register.
 * @param data the char representation of the data to be put in the UDR0 register
 */
void serial_putc(char data) {

	/* Wait for empty transmit buffer by checking the UDRE bit */
	while ((UCSR0A & 0b00100000) == 0){}
	
	/* Put data into transmit buffer; sends the data */
	UDR0 = data;
}

/// Transmit a string of characters over the serial connection
/**
 * Transmits a char* up to length 80 over the serial connection. 
 * The data is padded to be 80 chars long in order to preserve the frame of the
 * receiving terminal, assuming it is 80 characters wide.
 * @param data the char* to be transmitted
 */
void serial_puts(char* data){
	int i = 0;
	char buf[81];
	memset(buf, ' ', sizeof(char) * 81);
	sprintf(buf, "%s", data);
	for( ; i < 81; i++){
		serial_putc(buf[i]);
	}
}