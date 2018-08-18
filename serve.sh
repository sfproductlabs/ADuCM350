#!/bin/bash
JLinkGDBServer -device ADUCM350 -if SWD -speed 115.2 -jtagconf -1,-1 -autoconnect 1 
