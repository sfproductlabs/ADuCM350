# Firmware Compiler and Linker for the ADuCM350

This is the open source alternative using GCC to compile firmware for the ADuCM350. Use this if you don't want to spend thousands of dollars on an IAR compiler.

This software has been used and tested on:
* Linux (Debian Buster, Fedora 28)
* Segger EDU SWD

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

### Deploying for Production

* Build using the the "Release" configuration and use "upload.sh" to upload it to your device. This version can not be debugged.
* **My Debug version works differently from Release?** Whoa! The semi-hosting feature of gdb runs some instructions on the host machine. Don't be surprised if it mucks up on the release version (as it did for me). For example, there's no 64 bit division on the Cortex M-3 so execution will fail on overflows unless you implement your own (luckily we've rolled it in with our version - look at DIV64 in https://github.com/dioptre/ADuCM350/blob/imp4wire/src/Impedance4WireTest.c for an example). There may be other issues like this that pop up. 

## Contributions

* Create your own branch and push to here, then to do a pull request (https://help.github.com/articles/about-pull-requests/)
```
git checkout -b mybranch
git add -A :/
git commit -am "my branch is great"
git push origin mybranch
#then merge mybranch into our github repo using a pull request (above)
```
* Look for comments with AG or MVB for things that might be broken or missing. There was a duplicate interrupt in FLASH and CRC that was removed in CRC (find **ADI_INT_HANDLER(DMA_CRC_Int_Handler)**). Everything else should be working.

## Contributors

* Moritz von Buttlar
* Andrew Grosser (me)

## License

Where not specifically stated, this repository and all source code is licensed under the Apache License v2 (https://www.apache.org/licenses/LICENSE-2.0.html). Any contributions submitted to this project fall under the Apache License.

I'd like to thank Analog Devices for their permission to use and redistribute the original software (See /LICENSES/ADI.LABLAB.LICENSE) taken from their ADUCM350 SDK (http://www.analog.com/en/products/aducm350.html). Please assume we've modified the original versions.

Other licenses have been used to develop and source this software. Please see /LICENSES for more information and find specific license information for files in some of the headers. 

## Shameless Plug

Come check out what we're doing with the ADuCM350 and open source biomedical imaging at https://github.com/openeit, or our product development shop at http://sfproductlabs.com/

## Help I stuffed/bricked my chip
* Use the CM3WSD software that comes with the EVAL SDK (link above) board to erase it. Might need a friend with a Windows box to do this.
