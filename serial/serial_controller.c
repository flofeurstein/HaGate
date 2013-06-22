/*
 *  serial_controller.c
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *  
 *  Description:
 *
 */

#include "serial_controller.h"

/*
 *  serial_controller_open
 *
 *  Opens a serial port
 *
 *  @param serialPort describes the serial port to open
 *  @return the file descriptor on success or -1 on error
 */
int serial_controller_open(char* serialPort)
{
  int fd = 0; /* File descriptor for the port */
  struct termios options;

  /*
   * Get the current options for the port...
   */
  tcgetattr(fd, &options);

  /*
   * Set the baud rates to 38400...
   */
  cfsetispeed(&options, B38400);
  cfsetospeed(&options, B38400);

  /*
   * Enable the receiver and set local mode...
   */
  options.c_cflag |= (CLOCAL | CREAD);

  /*
   * Set the new options for the port...
   */
  tcsetattr(fd, TCSANOW, &options);

  /*
   * Setting the character size
   */
  options.c_cflag &= ~CSIZE; /* Mask the character size bits */
  options.c_cflag |= CS8;    /* Select 8 data bits */

  /*
   * Setting parity checking to no parity (8N1)
   */
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;

  /*
   * Setting HW flow control to enabled
   */
  options.c_cflag |= CRTSCTS;

  /*
   * Choosing input (canonical or raw)
   */
  //options.c_lflag |= (ICANON | ECHO | ECHOE); //canonical
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw

  /*
   * Choosing input
   */
  options.c_oflag &= ~OPOST; //raw

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open /dev/ttyUSB0 - ");
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
  }

  return (fd);
}

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
int serial_controller_read(int fd, uint8* pData, int dataLen)
{
  return read(fd, pData, dataLen);
}

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
int serial_controller_write(int fd, uint8* pData, int dataLen)
{
  return write(fd, pData, dataLen);
}

/*
 *  serial_controller_close_port
 *
 *  Closes a serial port
 *
 *  @param file descriptor of serial port to close
 *  @return 0 on success or -1 on error
 */
int serial_controller_close(int fd)
{
  if(close(fd) < 0)
  {
    perror("open_port: Unable to open /dev/ttyUSB0 - ");

    return -1;
  }

  return 0;
}
