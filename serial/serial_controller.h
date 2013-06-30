/*
 *  serial_controller.h
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *  
 *  Description:
 *
 */

#ifndef SERIAL_CONTROLLER_H_
#define SERIAL_CONTROLLER_H_

#include <stdlib.h>  /* Standard Library for malloc, free...*/
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include "hal_types.h"

#define HAGATE_SERIAL_INTERFACE    "/dev/ttyUSB0"

void serial_controller_setSigHandler(void (*sigHandler)(int));

/*
 *  serial_controller_open
 *
 *  Opens a serial port
 *
 *  @param serialPort describes the serial port to open
 *  @return the file descriptor on success or -1 on error
 */
int serial_controller_open(char* serialPort);

/*
 * serial_controller_read
 *
 * Reads from file (serial port) indicated by the fileDescriptor
 *
 * @fd file descriptor for the opened serial port
 * @pData read data into this location
 * @dataLen length of data to read
 * @return number of read bytes on success, negative on error
 */
int serial_controller_read(int fd, uint8* pData, int dataLen);

/*
 * serial_controller_write
 *
 * Writes to file (serial port) indicated by the fileDescriptor
 *
 * @fd file descriptor for the opened serial port
 * @pData write data from this location
 * @dataLen length of data to write
 * @return number of written bytes on success, negative on error
 */
int serial_controller_write(int fd, uint8* pData, int dataLen);

/*
 *  serial_controller_close_port
 *
 *  Closes a serial port
 *
 *  @param file descriptor of serial port to close
 *  @return 0 on success or -1 on error
 */
int serial_controller_close(int fileDescriptor);

#endif /* SERIAL_CONTROLLER_H_ */
