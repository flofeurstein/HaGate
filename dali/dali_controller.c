/*
 * dali_controller.c
 *
 *  Created on: Jun 20, 2013
 *      Author: flo
 */

#include "dali_controller.h"

/*
 * dali_controller_turnOffBroadcast
 *
 * Send a broadcast to turn off all devices
 */
void dali_controller_turnOffBroadcast()
{
  daliFwdFrame_t fwdFrame = {DALI_BROADCAST_CMD, DALI_CMD_OFF};

  dali_prot_sendFwdFrame(&fwdFrame);
}

/*
 * dali_controller_setToLevelBroadcast
 *
 * Send broadcast to set all devices to certain level
 *
 * @param level is the value to dim the lights to
 */
void dali_controller_setToLevelBroadcast(int level)
{
  daliFwdFrame_t fwdFrame = {DALI_BROADCAST_ARC_LEVEL, level};

  dali_prot_sendFwdFrame(&fwdFrame);
}

/*
 * dali_controller_turnOnBroadcast
 *
 * Send a broadcast to turn on all devices to the maximum level
 */
void dali_controller_turnOnBroadcast()
{
  dali_controller_setToLevelBroadcast(DALI_MAX_LEVEL);
}

/*
 * dali_controller_levelUpBroadcast
 *
 * Send a broadcast to increase the dim level on all devices
 */
void dali_controller_levelUpBroadcast()
{
  daliFwdFrame_t fwdFrame = {DALI_BROADCAST_CMD, DALI_CMD_UP};

  dali_prot_sendFwdFrame(&fwdFrame);
}

/*
 * dali_controller_levelDownBroadcast
 *
 * Send a broadcast to decrease the dim level on all devices
 */
void dali_controller_levelDownBroadcast()
{
  daliFwdFrame_t fwdFrame = {DALI_BROADCAST_CMD, DALI_CMD_DOWN};

  dali_prot_sendFwdFrame(&fwdFrame);
}

