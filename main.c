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
#include "hal_board.h"
#include <unistd.h>

int main(int argc, char **argv) {

  /*int i = 5;

  int hal_board_daliOutOpen();

  while(i>0)
  {
    hal_board_daliOut(1);

    sleep(1);

    hal_board_daliOut(0);

    sleep(1);
    i--;
  }

  int hal_board_daliOutClose();*/

  mt_controller_processHaGateCmd();

  //printf("stopped");

  return 0;
}

