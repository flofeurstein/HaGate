/*
 *  mt_controller.c
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *  
 *  Description:
 *
 */

#include "mt_controller.h"
#include "serial_controller.h"
#include "MT.h"
#include "dali_controller.h"
#include "MT_HAGATE.h"
#include <time.h>

/*
 * State of processing, 0 is not processing, 1 is processing,
 * static because now it can only be accessed within this file
 */
//static int isProcessing = 0;

static int mt_filedescriptor = 0;

/*
 * mt_controller_calcChecksum
 *
 * Calculate checksum for MT frame by xor-ing each byte, leave
 * the SOF and CHECKSUM fields, so start with the CMD
 * field
 * | SOP | DATALEN | CMD | DATA  | FCS | MT frame fields
 * |  1  |    1    |  2  | 0-LEN |  1  | Bytes
 *
 * @param cmdType is the type of the command [7:5] ored with the subsystem [4:0]
 * @param cmdId is the command ID
 * @param dataLen is the length of the data
 * @param pData pointer to the data of the message
 * @return the checksum of the message
 */
int mt_controller_calcChecksum(uint8 cmdType, uint8 cmdId, uint8 dataLen, uint8* pData)
{
  int checksum = cmdType ^ cmdId ^ dataLen;
  int i = 0;

  for(i = 0; i < dataLen; i++)
  {
    checksum ^= pData[i];
  }

  return checksum;
}

/*
 * mt_controller_open
 *
 * Opens the serial controller and sets the signal handler callback function.
 *
 * @param sigHandler is the callback function for the signal handler
 * @return 0 on success or -1 on error
 */
int mt_controller_open(void (*sigHandler)(int))
{
  serial_controller_setSigHandler(sigHandler);
  mt_filedescriptor = serial_controller_open(HAGATE_SERIAL_INTERFACE);
  return mt_filedescriptor;
}

/*
 * mt_controller_processHaGateCmd
 *
 * Process HaGate commands which are received from the serial
 * interface and send it to the dali module
 *
 * @return file descriptor of the serial control
 */
int mt_controller_processHaGateCmd()
{
  uint8 dataLen;
  //uint8 type;
  uint8 subsystem;
  uint8 hagateCommand;
  uint8* pData = NULL;
  uint8 mtHeader[MT_HEADER_LEN];
  int checksum;
  int success = 0;
  struct timespec sleepTime = {0, 1000};

  /*
   * If the serial controller is successfully opened, start processing
   */
  if(mt_filedescriptor > 0)
  {
    success = serial_controller_read(mt_filedescriptor, mtHeader, MT_HEADER_LEN);

    if(mtHeader[MT_SOP_POS] == MT_UART_SOF)
    {
      dataLen = mtHeader[MT_DATALEN_POS];
      //type = mtHeader[MT_CMD_POS_HI] & 0xF0;
      subsystem = mtHeader[MT_CMD_POS_HI] & 0xF;
      hagateCommand = mtHeader[MT_CMD_POS_LO];

      pData = (uint8*)malloc(dataLen + MT_CHKSM_SIZE);
      serial_controller_read(mt_filedescriptor, pData, dataLen + MT_CHKSM_SIZE);

      checksum = mt_controller_calcChecksum(mtHeader[MT_CMD_POS_HI], hagateCommand, dataLen, pData);

      /*
       * checking if the checksum is correct
       * the checksum is always after the last
       * DATA byte -> at position [dataLen]
       */
      if(checksum == pData[dataLen])
      {
        switch (subsystem)
        {
          case MT_RPC_SYS_HAGATE:
            switch (hagateCommand) {
              case MT_HAGATE_LIGHT_ON:
                dali_controller_turnOnBroadcast();
                break;
              case MT_HAGATE_LIGHT_OFF:
                dali_controller_turnOffBroadcast();
                break;
              case MT_HAGATE_LIGHT_TOGGLE:
                //not yet implemented
                break;
              case MT_HAGATE_LIGHT_DIM_LEVEL:
                dali_controller_setToLevelBroadcast(((zclLCMoveToLevel_t*)pData)->level);
                break;
              default:
                break;
            }
            break;
          default:
            break;
        }
      }

      if(pData != NULL)
      {
        free(pData);
        pData = NULL;
      }

      nanosleep(&sleepTime, NULL);
      tcflush(mt_filedescriptor, TCIFLUSH);
    }
  }

  return success;
}

/*
 * mt_controller_createSPIMessage
 *
 * Creates message in MT format.
 *
 * @param cmdType command type of the message
 * @param cmdId command ID of the message
 * @param datalen length of the data to be sent
 * @param pMessage pointer to memory where message should be stored
 * @param pData pointer to data that should be stored into the message
 */
void mt_controller_createSPIMessage(uint8 cmdType, uint8 cmdId, uint8 datalen, uint8* pMessage, uint8* pData)
{
  *pMessage++ = MT_UART_SOF;
  *pMessage++ = datalen;
  *pMessage++ = cmdType;
  *pMessage++ = cmdId;
  if(pData != NULL)
  {
    memcpy(pMessage, pData, datalen);
  }
}

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
int mt_controller_sendMessage(uint8 cmdType, uint8 cmdId, uint8 dataLen, uint8* pData)
{
  int success = 0;
  uint8 checksum = 0;
  uint8* pMessage = malloc(MT_HEADER_LEN + dataLen + MT_CHKSM_SIZE);

  if(pMessage != NULL)
  {
    mt_controller_createSPIMessage(cmdType, cmdId, dataLen, pMessage, pData);

    checksum = mt_controller_calcChecksum(cmdType, cmdId, dataLen, pData);

    pMessage[MT_HEADER_LEN + dataLen] = checksum;

    success = serial_controller_write(mt_filedescriptor, pMessage, (MT_HEADER_LEN + dataLen + MT_CHKSM_SIZE));

    free(pMessage);
    return success;
  }
  else
  {
    return -1;
  }

}

/*
 * cpyExtAddr
 *
 * Copy an extended address from source to destination.
 *
 * @param pDest pointer to destination memory
 * @param pSrc pointer to source memory
 */
void cpyExtAddr(uint8* pDest, uint8* pSrc)
{
  memcpy(pDest, pSrc, Z_EXTADDR_LEN);
}

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
int mt_controller_sendOnOffMessage(ZLongAddr_t* ieeeAddr, uint8 cmd)
{
  mtHaGateMsg_t msg;
  cpyExtAddr((uint8*)&(msg.IEEEAddr), (uint8*)ieeeAddr);
  msg.endpoint = SAMPLELIGHT_ENDPOINT;
  msg.dataLen = 0;
  msg.msgData = NULL;

  if(cmd < MT_HAGATE_LIGHT_ON || cmd > MT_HAGATE_LIGHT_TOGGLE)
  {
    return -1;
  }
  return mt_controller_sendMessage((MT_RPC_CMD_AREQ | MT_RPC_SYS_HAGATE), cmd, sizeof(mtHaGateMsg_t) + msg.dataLen, (uint8*)&msg);
}

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
int mt_controller_sendLevelControlMessage(ZLongAddr_t* ieeeAddr, uint8 level, uint16 transTime)
{
  mtHaGateMsg_t msg;
  zclLCMoveToLevel_t msgData;

  msgData.level = level;
  msgData.transitionTime = transTime;
  msgData.withOnOff = 0;

  cpyExtAddr((uint8*)&(msg.IEEEAddr), (uint8*)ieeeAddr);
  msg.endpoint = SAMPLELIGHT_ENDPOINT;
  msg.dataLen = sizeof(zclLCMoveToLevel_t);
  msg.msgData = (uint8*)&msgData;
  return mt_controller_sendMessage((MT_RPC_CMD_AREQ | MT_RPC_SYS_HAGATE), MT_HAGATE_LIGHT_ON, sizeof(mtHaGateMsg_t) + msg.dataLen, (uint8*)&msg);
}

/*
 * mt_controller_close
 *
 * Closes the serial controller.
 *
 * @return 0 on success or -1 on error
 */
int mt_controller_close()
{
  return serial_controller_close(mt_filedescriptor);
}
