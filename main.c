/*
 *  Main.c
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *  
 *  Description:
 *
 */

#include "mt_controller.h"
#include "MT.h"
#include "MT_HAGATE.h"
#include <unistd.h>

static int programRunning = 1;
static int messagePending = 0;

/***************************************************************************************************
 * IEEE addresses of devices
 ***************************************************************************************************/
ZLongAddr_t ieeeAddr_LSRLight = {0x00, 0x25, 0xCA, 0x02, 0x00, 0x02, 0x00, 0x64};
ZLongAddr_t ieeeAddr_LSRSwitch = {0x00, 0x25, 0xCA, 0x02, 0x00, 0x02, 0x00, 0x64};
ZLongAddr_t ieeeAddr_SmartRFLight = {0x00, 0x12, 0x4B, 0x00, 0x00, 0x0A, 0x12, 0xFA};
ZLongAddr_t ieeeAddr_SmartRFSwitch = {0x00, 0x12, 0x4B, 0x00, 0x00, 0x0A, 0x12, 0x88};

void signalHandler(int status)
{
  messagePending = 1;
}

int main(int argc, char **argv)
{
  //uint8 i = 10;

  mt_controller_open(&signalHandler);

  while(programRunning)
  {
    mt_controller_sendOnOffMessage(&ieeeAddr_LSRLight, MT_HAGATE_LIGHT_TOGGLE);

    if(messagePending)
    {
      mt_controller_processHaGateCmd();
      messagePending = 0;

    }


  }


  mt_controller_close();

  return 0;
}

