Mbed ADuCM350 GCC Native compilation

* install mbed-cli 0.4.7 
* Setup Env Variables
```
mbed config -G MBED_ARM_PATH /home/xxx/ARM_Compiler_5.06u6/bin
mbed config -G MBED_GCC_ARM_PATH /home/xxx/gcc-arm-none-eabi-7-2017-q4-major/bin/
```
* Compile 
```
mbed compile -m EVAL_ADUCM350 -t GCC_ARM
```


