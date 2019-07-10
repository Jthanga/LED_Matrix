#!/bin/bash

cd /lib/firmware

echo keypad-00A0 > $SLOTS 
echo bone-pwm-P8_13 > $SLOTS
echo BB-BONE-BACONE > $SLOTS

cd /sys/class/gpio

echo 5 > export
echo 4 > export
echo 2 > export

echo 111 > export
echo 113 > export
echo 110 > export

echo 66 > export
echo 67 > export
echo 69 > export
echo 68 > export
echo 47 > export
echo 46 > export
echo 27 > export
echo 65 > export

cd gpio47 
echo out > direction
cd ..

cd gpio46
echo out > direction
cd ..

cd gpio27 
echo out > direction
cd ..

cd gpio65 
echo out > direction
cd ..

cd gpio5 
echo out > direction
cd ..

cd gpio4
echo out > direction
cd ..

cd gpio2 
echo out > direction
cd ..

cd gpio111
echo out > direction
cd ..

cd gpio113
echo out > direction
cd ..
 
 cd gpio110 
 echo out < direction
 cd ..
 
 cd 
  



