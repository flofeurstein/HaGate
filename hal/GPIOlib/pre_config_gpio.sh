#!/bin/sh
dali_input_port="138"
dali_output_port="137"

#activate gpio ports
echo ${dali_output_port} > /sys/class/gpio/export
echo ${dali_input_port} > /sys/class/gpio/export

#set direction and value to dialout group and make it read and writeable
chgrp dialout /sys/class/gpio/gpio${dali_input_port}/direction /sys/class/gpio/gpio${dali_input_port}/value
chgrp dialout /sys/class/gpio/gpio${dali_output_port}/direction /sys/class/gpio/gpio${dali_output_port}/value
chmod 660 /sys/class/gpio/gpio${dali_input_port}/direction /sys/class/gpio/gpio${dali_input_port}/value
chmod 660 /sys/class/gpio/gpio${dali_output_port}/direction /sys/class/gpio/gpio${dali_output_port}/value

