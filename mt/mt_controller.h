/*
 *  mt_controller.h
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *  
 *  Description:
 *
 */

#ifndef MT_CONTROLLER_H_
#define MT_CONTROLLER_H_

#include "hal_types.h"

#define HEADER_LEN        4
#define SOF_POS           0
#define DATALEN_POS       1
#define CMD_POS_HI        2
#define CMD_POS_LO        3

/*
 * Nr of bytes for checksum, carriage return and line feed
 */
#define CHKSM_CR_LF_SIZE  3

typedef struct
{
  uint8 level;
  uint16 transitionTime;
  uint8 withOnOff;
}zclLCMoveToLevel_t;

/*
 * mt_controller_processHaGateCmd
 *
 * Process HaGate commands which are received from the serial
 * interface and send it to the dali module
 *
 * @return file descriptor of the serial control
 */
int mt_controller_processHaGateCmd();

/*
 * mt_controller_stopProcessing
 *
 * Stop the processing of HaGate commands
 */
void mt_controller_stopProcessing();

#endif /* MT_CONTROLLER_H_ */
