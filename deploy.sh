#!/bin/bash

# Build and deploy to BeagleBone debug device

echo "BUILDING"
make

echo "DEPLOYING TO DEVICE"
UNAME=debian
SSH_IP=192.168.7.2
LOCATION=/home/debian/bin

scp sim.out $UNAME@$SSH_IP:$LOCATION
