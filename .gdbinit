set auto-load safe-path /
# Connect to the GDB Server
target remote :2331
file Debug/firm.elf
monitor reset
monitor halt
monitor endian little
monitor speed auto
monitor flash device ADUCM350
monitor semihosting enable
monitor reset halt
load
#monitor reset init
continue
