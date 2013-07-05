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

/*
 * mt_controller_open
 *
 * Opens the serial controller and sets the signal handler callback function.
 *
 * @param sigHandler is the callback function for the signal handler
 * @return 0 on success or -1 on error
 */
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

/*
 * mt_controller_sendMessage
 *
 * Sends a MT formatted message to the serial controller.
 *
 * @param cmdType command type of the message
 * @param cmdId command ID of the message
 * @param datalen length of the data to be sent
 * @param pData pointer to data that should be sent with the message
 * @return success state, successful if >= 0
 */
int mt_controller_sendMessage(uint8 cmdType, uint8 cmdId, uint8 dataLen, uint8* pData);

/*
 * mt_controller_sendOnOffMessage
 *
 * Send on or off or toggle message to certain ieee address.
 *
 * @param ieeeAddr is the extended (IEEE) address of the node where the
 * message should be sent to
 * @param cmd is the command that should be sent to the node
 * @return success state, successful if >= 0
 */
int mt_controller_sendOnOffMessage(ZLongAddr_t* ieeeAddr, uint8 cmd);

/*
 * mt_controller_sendLevelControlMessage
 *
 * Send level control message to certain node
 *
 * @param ieeeAddr is the extended (IEEE) address of the node where the
 * message should be sent to
 * @param level is the level the node should be set to
 * @param transTime is the transistion time for the level change
 * @return success state, successful if >= 0
 */
int mt_controller_sendLevelControlMessage(ZLongAddr_t* ieeeAddr, uint8 level, uint16 transTime);

/*
 * mt_controller_close
 *
 * Closes the serial controller.
 *
 * @return 0 on success or -1 on error
 */
int mt_controller_close();

#endif /* MT_CONTROLLER_H_ */
