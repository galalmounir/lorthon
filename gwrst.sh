#!/bin/sh

# Set GPIO 7 to low, wait for 0.5 seconds, then set it back to high
gpioset -l gpiochip0 7=0

sleep 0.1

gpioset -l gpiochip0 7=1