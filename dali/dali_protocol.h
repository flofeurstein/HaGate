/*
 * dali_protocol.h
 *
 *  Created on: Jun 20, 2013
 *      Author: flo
 */

#ifndef DALI_PROTOCOL_H_
#define DALI_PROTOCOL_H_

#include "hal_types.h"

/*
 * Length of forward and backward frame in bits
 */
#define DALI_FWDFRAME_LEN           19
#define DALI_BWDFRAME_LEN           11
#define NR_BITS_IN_BYTE             8

#define DALI_STARTBIT               1
#define DALI_STOPBITS               0x3

/*
 * Polarity of HIGH and LOW signals
 */
#define DALI_HIGH                   1
#define DALI_LOW                    0

/*
 * DALI Broadcast Address
 */
#define DALI_BROADCAST_CMD          0xFF
#define DALI_BROADCAST_ARC_LEVEL    0xFE

/*
 * DALI Commands
 */
#define DALI_MAX_LEVEL              0xFE

#define DALI_CMD_OFF                0x0
#define DALI_CMD_UP                 0x1
#define DALI_CMD_DOWN               0x2

#define DALI_QUERY_LAMP_POWER_ON    0x93

#pragma pack(1)
typedef struct
{
  uint8 startBit:1;
  uint8 address;
  uint8 data;
  uint8 stopBits:2;
}daliFwdFrame_t;
#pragma pack()

#pragma pack(1)
typedef struct
{
  uint8 startBit:1;
  uint8 data;
  uint8 stopBits:2;
}daliBwdFrame_t;
#pragma pack()

/*
 * dali_prot_sendManchesterEncodedFwdFrame
 *
 * Encode data frame via Manchester Encoding and send it to board
 * configuration in hal
 *
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendManchesterEncodedFwdFrame(daliFwdFrame_t* pData);

#endif /* DALI_PROTOCOL_H_ */
