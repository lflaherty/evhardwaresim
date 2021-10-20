# README #

Hardware simulator to run on BeagleBone Black with embedded Linux.

The software tests ECU hardware, which act as devices under test.

With the custom expansion board hardware and this software, the BBB platform can stimulate I/O on the ECU hardware to run tests and verify operation in a hardware-in-the-loop (HIL) environment.

# Pre-requisities #

* This project is intended to be run on ARM based Linux hardware, so the cross-compiler is needed: `sudo apt install g++-arm-linux-gnueabihf`
* The device tree overlay in `beaglebone_scripts/` must be compiled and loaded onto the device
* `/boot/uEnv.txt` must be updated to load the correct I/O setup (i.e. CAN bus, serial, SPI, and the appropriate GPIOs). (See `beaglebone_scripts/uEnv.txt` for reference.)

