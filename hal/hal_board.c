/*
 *  hal_board.c
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *
 *  Description:
 */

#include "hal_board.h"
#include "gpio.h"

/*
 * hal_board_daliOutOpen
 *
 * Open the output port of the DALI connector
 *
 * @return >= 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliOutOpen()
{
  int error = 0;

  if (gpio_open(DALI_OUT_PORT, GPIO_WRITE) < 0)
  {
    perror("Unable to open PIN file");
    error =  -1;
  }

  return error;
}

/*
 * hal_board_daliInOpen
 *
 * Open the input port of the DALI connector
 *
 * @return >= 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliInOpen()
{
  int error = 0;

  if (gpio_open(DALI_IN_PORT, GPIO_READ) < 0)
  {
    perror("Unable to open PIN file");
    error = -1;
  }

  return error;
}

/*
 * hal_board_daliOut
 *
 * Writes value to DALI output port
 *
 * @param level sets output port to HIGH or LOW level (0 is LOW,
 *              anything else is HIGH)
 * @return >= 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliOut(int level)
{
  return gpio_write(DALI_OUT_PORT,level);
}

/*
 * hal_board_daliIn
 *
 * Reads value from DALI input port
 *
 * @return the current value of the input port if successful,
 *         < 0 if not successful
 */
int hal_board_daliIn()
{
  return gpio_read(DALI_IN_PORT);
}

/*
 * hal_board_daliOutClose
 *
 * Closes dali output port
 *
 * @return >= 0 on success, < 0 on error
 */
int hal_board_daliOutClose()
{
  return gpio_close(DALI_OUT_PORT);
}

/*
 * hal_board_daliInClose
 *
 * Closes dali input port
 *
 * @return >= 0 on success, < 0 on error
 */
int hal_board_daliInClose()
{
  return gpio_close(DALI_IN_PORT);
}

