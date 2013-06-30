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
#include <sys/signal.h>

void sigHandler(int status)
{
  volatile int i = 1;
  i++;
}

static void (*sigHandlerCB)(int) = NULL;

void serial_controller_setSigHandler(void (*sigHandler)(int))
{
  sigHandlerCB = sigHandler;
}

/*
 *  serial_controller_openBlocking
 *
 *  Opens a serial port
 *
 *  @param serialPort describes the serial port to open
 *  @return the file descriptor on success or -1 on error
 */
int serial_controller_openBlocking(char* serialPort)
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
  //tcsetattr(fd, TCSANOW, &options);

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
  //options.c_cflag &= ~CSIZE;
  //options.c_cflag |= CS8;

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

  /*
   * Set the new options for the port...
   */
  tcsetattr(fd, TCSANOW, &options);

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
  struct sigaction sigAction;

  fd = open(HAGATE_SERIAL_INTERFACE, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    printf("open_port: Unable to open /dev/ttyUSB0 - ");
    return fd;
  }


  /*
   * Setting up signal handler
   */
  if(sigHandlerCB == NULL){
    /*
    * No callback function defined.
    */

    printf("No callback function defined, use serial_controller_setSigHandler");
    return -1;
  }
  sigAction.sa_handler = sigHandlerCB;
  //sigAction.sa_mask = sigmask(0);
  sigAction.sa_flags = 0;
  sigAction.sa_restorer = NULL;
  sigaction(SIGIO,&sigAction,NULL);

  fcntl(fd, F_SETOWN, getpid());
  fcntl(fd, F_SETFL, O_ASYNC);

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
  //tcsetattr(fd, TCSANOW, &options);

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
  //options.c_cflag &= ~CSIZE;
  //options.c_cflag |= CS8;

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

  options.c_cc[VTIME] = 0;
  options.c_cc[VMIN] = 1;
  //options.c_cc[VKILL] = '\H';

  tcflush(fd, TCIFLUSH);
  /*
   * Set the new options for the port...
   */
  tcsetattr(fd, TCSANOW, &options);

  return fd;
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
    printf("close_port: Unable to close /dev/ttyUSB0 - ");

    return -1;
  }

  return 0;
}
