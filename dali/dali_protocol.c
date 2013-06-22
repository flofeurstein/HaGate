/*
 * dali_protocol.c
 *
 *  Created on: Jun 20, 2013
 *      Author: flo
 */

#include <stdio.h>

#include "dali_protocol.h"
#include "hal_board.h"

/*
 * checkBit
 *
 * Checks a certain bit in a byte
 *
 * @param pos is the position of the bit to be checked
 * @param byte is the byte where a certain bit should be checked
 * @return value of the bit, 0x1 if the bit is 1 and 0x0 if the bit is 0
 */
uint8 checkBit(uint8 pos, uint8 byte)
{
  return ((byte & (1 << pos)) >> pos);
}

void dali_prot_sendBitHigh()
{
  hal_board_daliOut(DALI_HIGH);
  printf("1");
}

void dali_prot_sendBitLow()
{
  hal_board_daliOut(DALI_LOW);
  printf("0");
}

void dali_prot_sendManchesterBit(uint8 bit)
{
  if(bit == 1)
  {
    //set output signal low
    dali_prot_sendBitLow();
    //wait mid-bit time
    //set output signal high
    dali_prot_sendBitHigh();
    //wait mid-bit time
  }
  else
  {
    //set output signal high
    dali_prot_sendBitHigh();
    //wait mid-bit time
    //set output signal low
    dali_prot_sendBitLow();
    //wait mid-bit time
  }
}

void dali_prot_sendStopBits()
{
  dali_prot_sendBitHigh();
  //wait mid-bit time
  dali_prot_sendBitHigh();
  //wait mid-bit time
  dali_prot_sendBitHigh();
  //wait mid-bit time
  dali_prot_sendBitHigh();
}

/*
 * dali_prot_sendManchesterEncodedFwdFrame
 *
 * Encode data frame via Manchester Encoding and send it to board
 * configuration in hal
 *
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendManchesterEncodedFwdFrame(daliFwdFrame_t* pData)
{
  int success = 0;
  int i = 0;
  uint8 bit = 0;

  success = hal_board_daliOutOpen();
  //success = 1;

  if(success >= 0)
  {
    /*
     * send startbit
     */
    bit = checkBit(0, pData->startBit);
    dali_prot_sendManchesterBit(bit);

    /*
     * send address bits
     */
    for(i = NR_BITS_IN_BYTE-1; i >= 0; i--)
    {
      bit = checkBit(i, pData->address);
      dali_prot_sendManchesterBit(bit);
    }


    /**********************************test***********************TODO delete********************************

    if(pData->data == 0x0)
    {
      hal_board_daliOut(0);
    }
    else
    {
      hal_board_daliOut(1);
    }

    *********************************end test***********************TODO delete*****************************/

    /*
     * send data bits
     */
    for(i = NR_BITS_IN_BYTE-1; i >= 0; i--)
    {
      bit = checkBit(i, pData->data);
      dali_prot_sendManchesterBit(bit);
    }

    /*
     * send stop bits
     */
    dali_prot_sendStopBits();
  }

  success = hal_board_daliOutClose();

  return success;
}

void dali_prot_manchesterDecoding(uint8* pData, uint8* pManchesterData)
{

}
