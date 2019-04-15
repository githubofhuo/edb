#!/bin/bash
machine=`uname -m | grep '64'`
if [ a"$machine" == "a" ]; then
  bitlevel=32
else
  bitlevel=64
fi
cd boost
./bootstrap.sh --enable-pic
./bjam link=static threading=multi variant=release address-model=$bitlevel toolset=gcc runtime-link=static 