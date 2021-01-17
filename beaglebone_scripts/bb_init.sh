#!/bin/sh -

# Initialize BeagleBone hardware

# Note #1 this script needs to be run as su
# Note #2 requires overlays to be expressed in /boot/uEnv.txt


############## Init CAN bus #############
echo "[bb_init.sh]  Starting CAN bus"
# CAN_BITRATE=1000000
CAN_BITRATE=500000
# CAN_BITRATE=250000

# Check whether CAN is running
temp=$(ifconfig | grep can0)
can_grep=$?

if [ $can_grep -eq 1 ]; then
    # echo BB-CAN1 > /sys/devices/platform/bone_capemgr/slots
    modprobe can
    modprobe can-dev
    modprobe can-raw

    #ip link set can0 up type can bitrate $CAN_BITRATE loopback on   # use this line instead for single device debugging (loopback => packets read on same host)
    ip link set can0 up type can bitrate $CAN_BITRATE restart-ms 100
    ifconfig can0 txqueuelen 1000
    ifconfig can0 up
else
    echo "[bb_init.sh]  CAN bus already active"
fi

############## Init SPI bus ##############
# echo BB-SPIDEV0 > /sys/devices/platform/bone_capemgr/slots

############## Init GPIO    ##############
echo "[bb_init.sh]"
echo "[bb_init.sh]  Starting GPIOs"
# Enable all in use GPIO pins
enable_pin () {
    pinNum=$1

    # Only enable if not already enabled
    if [ ! -d /sys/class/gpio/gpio$pinNum ]; then
        echo "[bb_init.sh]  Enabling GPIO $pinNum"
        echo $pinNum > /sys/class/gpio/export
    else
        echo "[bb_init.sh]  GPIO $pinNum already enabled"
    fi
}

enable_pin 49
enable_pin 117
enable_pin 115
enable_pin 113
enable_pin 111
enable_pin 112
