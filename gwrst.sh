#!/bin/sh

# This script is intended to be used to reset RHF0M301 module
# SX1301 chip need to be mannually reset before working

RHF0M301_RESET_PIN=7
WAIT_TIME=0.1

if [ $(id -u) -ne "0" ]
then
    echo "ERROR: Please run as root"
    exit 1
fi

if [ ! -d /sys/class/gpio/gpio$RHF0M301_RESET_PIN ]
then
    echo "$RHF0M301_RESET_PIN" > /sys/class/gpio/export
    sleep $WAIT_TIME
fi
echo "out" > /sys/class/gpio/gpio$RHF0M301_RESET_PIN/direction
sleep $WAIT_TIME
echo "1" > /sys/class/gpio/gpio$RHF0M301_RESET_PIN/value
sleep $WAIT_TIME
echo "0" > /sys/class/gpio/gpio$RHF0M301_RESET_PIN/value

echo "Gateway reset successfully"

exit 0
