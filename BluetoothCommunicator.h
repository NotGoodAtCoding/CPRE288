/*
 * BluetoothCommunicator.h
 *
 * Created: 4/13/2016 12:29:11 PM
 *  Author: sakraus
 */ 
#ifndef BLUETOOTH_COMMUNICATOR_H
#define BLUETOOTH_COMMUNICATOR_H

// Initialize serial communication over bluetooth
void serial_init(void);

//Receive data from the serial connection
unsigned char serial_getc();

//Transmit Data over the Serial connection
void serial_putc(char data);

// Transmit a string of characters over the serial connection
void serial_puts(char* data);

void serial_put_and_clear(char* data);

#endif