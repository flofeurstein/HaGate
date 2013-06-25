#!/bin/sh

#put this into /etc/rc.local

###########################
#       GPIO CONFIG       #
###########################
insmod /home/ubuntu/drivers/dali/dali_drv.ko
chgrp dialout /dev/dali_drv
chmod 660 /dev/dali_drv

###########################
#      SERIAL CONFIG      #
###########################
stty -F /dev/ttyUSB0 cs8 38400 -icrnl -imaxbel -opost -onlcr -isig -icanon -echo min 100 time 2 line 0 kill ^H

