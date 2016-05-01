/*
 * BluetoothCommunicator.h
 *
 * Created: 4/13/2016 12:29:11 PM
 *  Author: sakraus
 */ 
#ifndef BLUETOOTH_COMMUNICATOR_H
#define BLUETOOTH_COMMUNICATOR_H

/// Initialize serial communication over bluetooth
/**
 * Set the UBBR0H and UBRR0L registers to the BAUD rate 34. 
 * Set the UCSR0A register to enable double speed mode.
 * Set the UCSR0C register to set the frame format: 8 data bits, 2 stop bits
 * Set the UCSR0B register to enable receiver and transmitter.
 */
void serial_init(void);

/// Receive data from the serial connection
/**
 * Waits for a recieve complete flag in the UCSR0A register, then
 * returns the data in the UDR0 register and clears it.
 * @return the char representation of the data in the UDR0 register
 */
unsigned char serial_getc();

/// Transmit Data over the Serial connection
/**
 * Wait for empty transmit buffer by checking the UDRE bit 
 * puts the data in the UDR0 register.
 * @param data the char representation of the data to be put in the UDR0 register
 */
void serial_putc(char data);

/// Transmit a string of characters over the serial connection
/**
 * Transmits a char* up to length 80 over the serial connection. 
 * The data is padded to be 80 chars long in order to preserve the frame of the
 * receiving terminal, assuming it is 80 characters wide.
 * @param data the char* to be transmitted
 */
void serial_puts(char* data);

#endif