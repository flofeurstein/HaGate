/*
 *  hal_board.h
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *
 *  Description: Replace/modify this for use with other board
 */

#ifndef HAL_BOARD_H_
#define HAL_BOARD_H_

#define GPIO_138            138
#define GPIO_137            137

#define EXP_HEADER_PIN_5    GPIO_138
#define EXP_HEADER_PIN_7    GPIO_137

#define DALI_IN_PORT        EXP_HEADER_PIN_5
#define DALI_OUT_PORT       EXP_HEADER_PIN_7

/*
 * hal_board_daliOutOpen
 *
 * Open the output port of the DALI connector
 *
 * @return > 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliOutOpen();

/*
 * hal_board_daliInOpen
 *
 * Open the input port of the DALI connector
 *
 * @return > 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliInOpen();

/*
 * hal_board_daliOut
 *
 * Writes value to DALI output port
 *
 * @param level sets output port to HIGH or LOW level (0 is LOW,
 * anything else is HIGH)
 * @return >= 0 if opened successful, < 0 if opened with error
 */
int hal_board_daliOut(int level);

/*
 * hal_board_daliIn
 *
 * Reads value from DALI input port
 *
 * @return the current value of the input port if successful,
 *         < 0 if not successful
 */
int hal_board_daliIn();

/*
 * hal_board_daliOutClose
 *
 * Closes dali output port
 *
 * @return >= 0 on success, < 0 on error
 */
int hal_board_daliOutClose();

/*
 * hal_board_daliInClose
 *
 * Closes dali input port
 *
 * @return >= 0 on success, < 0 on error
 */
int hal_board_daliInClose();

#endif /* HAL_BOARD_H_ */
