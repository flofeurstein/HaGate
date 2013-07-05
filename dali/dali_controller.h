/*
 *  dali_controller.h
 *
 *  Created on: Jun 21, 2013
 *  Author: Florian Feurstein
 *
 *  Description:
 *
 */

#ifndef DALI_CONTROLLER_H_
#define DALI_CONTROLLER_H_

#include "dali_protocol.h"

/*
 * dali_controller_turnOffBroadcast
 *
 * Send a broadcast to turn off all devices
 */
void dali_controller_turnOffBroadcast();

/*
 * dali_controller_setToLevelBroadcast
 *
 * Send broadcast to set all devices to certain level
 *
 * @param level is the value to dim the lights to
 */
void dali_controller_setToLevelBroadcast(int level);

/*
 * dali_controller_turnOnBroadcast
 *
 * Send a broadcast to turn on all devices to the maximum level
 */
void dali_controller_turnOnBroadcast();

/*
 * dali_controller_levelUpBroadcast
 *
 * Send a broadcast to increase the dim level on all devices
 */
void dali_controller_levelUpBroadcast();

/*
 * dali_controller_levelDownBroadcast
 *
 * Send a broadcast to decrease the dim level on all devices
 */
void dali_controller_levelDownBroadcast();

#endif /* DALI_CONTROLLER_H_ */
