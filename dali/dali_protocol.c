/*
 *  dali_protocol.c
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *
 *  Description:
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "dali_protocol.h"


/*
 * dali_prot_sendFwdFrame
 *
 * Send a DALI forward frame to the dali driver
 *
 * @param pData dali forward frame structure (data and address byte)
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendFwdFrame(daliFwdFrame_t* pData)
{
  int fd = 0;

  fd = open("/dev/dali_drv", O_RDWR);
  //success = 1;

  if(fd >= 0)
  {
    write(fd, (char*)pData, DALI_FWDFRAME_LEN);
  }

  close(fd);

  //wait at least 9.17 ms before sending another frame

  return fd;
}


/*
 * dali_prot_sendBwdFrame
 *
 * Send a DALI forward frame to the dali driver
 *
 * @param pData dali backward frame structure (data byte)
 * @return success state, > 0 if successful, < 0 if error
 */
int dali_prot_sendBwdFrame(daliFwdFrame_t* pData)
{
  int fd = 0;

  fd = open("/dev/dali_drv", O_RDWR);
  //success = 1;

  if(fd >= 0)
  {
    write(fd, (char*)pData, DALI_BWDFRAME_LEN);
  }

  close(fd);

  //wait at least 9.17 ms before sending another frame

  return fd;
}

