#!/bin/bash
screen /dev/ttyUSB0
#minicom --baudrate 115200 --device /dev/ttyUSB0
#By default, minicom has hardware flow control turned on. In this mode, you’ll find that letters typed in the terminal don’t reach your module. You can turn it off.
#ctrl-A -> O -> serial port setup -> F