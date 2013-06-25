/*
 *  dali_protocol.h
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *
 *  Description:
 *
 */

#ifndef DALI_PROTOCOL_H_
#define DALI_PROTOCOL_H_

#include "hal_types.h"

/*
 * Length of forward and backward frame in bytes
 */
#define DALI_FWDFRAME_LEN           2
#define DALI_BWDFRAME_LEN           1

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
  uint8 address;
  uint8 data;
}daliFwdFrame_t;
#pragma pack()

#pragma pack(1)
typedef struct
{
  uint8 data;
}daliBwdFrame_t;
#pragma pack()

/*
 * dali_prot_sendFwdFrame
 *
 * Send a DALI forward frame to the dali driver
 *
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendFwdFrame(daliFwdFrame_t* pData);

/*
 * dali_prot_sendBwdFrame
 *
 * Send a DALI forward frame to the dali driver
 *
 * @param pData dali backward frame structure (data byte)
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendBwdFrame(daliFwdFrame_t* pData);

#endif /* DALI_PROTOCOL_H_ */
