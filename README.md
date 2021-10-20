# README #

Hardware simulator to run on BeagleBone Black with embedded Linux.

The software tests ECU hardware, which act as devices under test.

With the custom expansion board hardware and this software, the BBB platform can stimulate I/O on the ECU hardware to run tests and verify operation in a hardware-in-the-loop (HIL) environment.

# Pre-requisities #

* This project is intended to be run on ARM based Linux hardware, so the cross-compiler is needed: `sudo apt install g++-arm-linux-gnueabihf`
* View `beaglebone_scripts/README.md` for device setup instructions.

