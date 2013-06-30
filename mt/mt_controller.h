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

#include "ZComDef.h"
#include "hal_types.h"

#define MT_HEADER_LEN        4
#define MT_SOP_POS           0
#define MT_DATALEN_POS       1
#define MT_CMD_POS_HI        2
#define MT_CMD_POS_LO        3

/*
 * Nr of bytes for checksum, carriage return and line feed
 */
#define MT_CHKSM_SIZE        1

typedef struct
{
  uint8                 cmdID;
} zclOnOff_t;

typedef struct
{
  uint8                 level;          // new level to move to
  uint16                transitionTime; // time to take to move to the new level (in seconds)
  uint8                 withOnOff;      // with On/off command
} zclLCMoveToLevel_t;

int mt_controller_open(void (*sigHandler)(int));

/*
 * mt_controller_processHaGateCmd
 *
 * Process HaGate commands which are received from the serial
 * interface and send it to the dali module
 *
 * @return file descriptor of the serial control
 */
int mt_controller_processHaGateCmd();

int mt_controller_sendMessage(uint8 cmdType, uint8 cmdId, uint8 dataLen, uint8* pData);

int mt_controller_close();

#endif /* MT_CONTROLLER_H_ */
