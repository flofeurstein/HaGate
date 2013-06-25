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

/*
 * State of processing, 0 is not processing, 1 is processing,
 * static because now it can only be accessed within this file
 */
static int isProcessing = 0;

/*
 * mt_controller_calcChecksum
 *
 * Calculate checksum for MT frame by xor-ing each byte, leave
 * the SOF, DATALEN and CHECKSUM fields, so start with the CMD
 * field
 * | SOP | DATALEN | CMD | DATA  | FCS | MT frame fields
 * |  1  |    1    |  2  | 0-LEN |  1  | Bytes
 *
 * @param pMtHeader pointer to the header of the message
 * @param pData pointer to the data of the message
 * @return the checksum of the message
 */
int mt_controller_calcChecksum(uint8* pMtHeader, uint8* pData)
{
  int checksum = pMtHeader[CMD_POS_HI] ^ pMtHeader[CMD_POS_LO];;
  int dataLen = pMtHeader[DATALEN_POS];
  int i = 0;

  for(i = 0; i < dataLen; i++)
  {
    checksum ^= pData[i];
  }

  return checksum;
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
  int fd = serial_controller_open(HAGATE_SERIAL_INTERFACE);
  uint8 dataLen;
  uint8 type;
  uint8 subsystem;
  uint8 hagateCommand;
  uint8* pData = NULL;
  uint8 mtHeader[HEADER_LEN];
  int checksum;

  /*
   * If the serial controller is successfully opened, start processing
   */
  if(fd > 0)
  {
    isProcessing = 1;
  }

  while(isProcessing)
  {
    serial_controller_read(fd, mtHeader, HEADER_LEN);
    //printf("0x%X ", buff[0]);
    //printf("%c ", (char)buff[0]);

    if(mtHeader[SOF_POS] == MT_UART_SOF)
    {
      dataLen = mtHeader[DATALEN_POS];
      type = mtHeader[CMD_POS_HI] & 0xF0;
      subsystem = mtHeader[CMD_POS_HI] & 0xF;
      hagateCommand = mtHeader[CMD_POS_LO];

      pData = (uint8*)malloc(dataLen + CHKSM_CR_LF_SIZE);
      serial_controller_read(fd, pData, dataLen + CHKSM_CR_LF_SIZE);
      //serial_controller_read(fd, mtHeader, dataLen + CHKSM_CR_LF_SIZE);

      checksum = mt_controller_calcChecksum(mtHeader, pData);

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
    }
  }

  serial_controller_close(fd);

  return fd;
}

/*
 * mt_controller_stopProcessing
 *
 * Stop the processing of HaGate commands
 */
void mt_controller_stopProcessing()
{
  isProcessing = 0;
}
