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
#include <unistd.h>

static int programRunning = 1;
static int messagePending = 0;

void signalHandler(int status)
{
  messagePending = 1;
}

int main(int argc, char **argv)
{
  uint8 i = 10;

  uint8 data[1];

  mt_controller_open(&signalHandler);

  while(programRunning)
  {
    data[0] = i;
    //mt_controller_sendMessage((MT_RPC_CMD_AREQ | MT_RPC_SYS_HAGATE), MT_HAGATE_LIGHT_ON, 1, data);

    if(messagePending)
    {
      mt_controller_processHaGateCmd();
      messagePending = 0;
      mt_controller_sendMessage((MT_RPC_CMD_AREQ | MT_RPC_SYS_HAGATE), MT_HAGATE_LIGHT_ON, 1, data);
    }

    if(i == 1)
    {
      programRunning = 0;
    }
    sleep(2);
    //i--;
  }


  mt_controller_close();

  return 0;
}

