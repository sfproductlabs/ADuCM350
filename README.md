# Firmware Compiler and Linker for the ADuCM350

This is the open source alternative using GCC to compile firmware for the ADuCM350. Use this if you don't want to spend thousands of dollars on an IAR compiler.

This software has been used and tested on:
* Linux (Debian Buster, Fedora 28)
* Segger EDU

## Instructions

* Install Eclipse
```
Version: Oxygen.3a Release (4.7.3a)
Build id: 20180405-1200
```
* Install the wonderful GNU MCU Eclipse Plugin (https://gnu-mcu-eclipse.github.io/).
* Install the EmbSys RegView (http://embsysregview.sourceforge.net/) - optional.
* Download the latest gnu-arm compiler for your host platform (gcc-arm-none-eabi-7-2017-q4-major tested)
* Tell eclipse where to find the compiler
* Connect your segger to the Eclipse Debugger
* Debug!

If you use debian you might be able to just run my eclipse with this repo:
https://github.com/dioptre/eclipse


## Contributions

* Create your own branch and push to here
```
git checkout -b mybranch
git add -A :/
git commit -am "my branch is great"
git push origin mybranch
```
* Look for comments with AG or MVB for things that might be broken or missing. There was a duplicate interrupt in FLASH and CRC that was removed in CRC (find **ADI_INT_HANDLER(DMA_CRC_Int_Handler)**). Everything else should be working.

## Contributors

* Moritz von Buttlar
* Andrew Grosser

## License

Where not specifically stated, this repository and all source code is licensed under the Apache License v2 (https://www.apache.org/licenses/LICENSE-2.0.html). Any contributions submitted to this project fall under the Apache License.

I'd like to thank Analog Devices for their permission to use and redistribute the original software (See /LICENSES/ADI.LABLAB.LICENSE) taken from their ADUCM350 SDK (http://www.analog.com/en/products/aducm350.html).

Other licenses have been used to develop and source this software. Please see /LICENSES for more information and find specific license information for files in some of the headers. 

## Shameless Plug

Come check out what we're doing with the ADuCM350 and open source biomedical imaging at https://github.com/openeit
