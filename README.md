# BaremetalRPI
A C++ implementation of a bare metal OS for the Raspberry PI 3a+.

## Credits
Credits to bztsrc on Github for his C tutorials
(https://github.com/bztsrc/raspi3-tutorial/blob/master/), which helped enormously for the
initial setup (mainly start.S and link.ld). It is another helpful resource which is worth
a look at.

Also credits to PeterLemon for his assembly tutorials which also helped enormously: https://github.com/PeterLemon/RaspberryPi/blob/master

The OsDEV wiki also contains helpful resources that I have looked at and used (such as the stacktracing)
https://wiki.osdev.org/

## Compiler
This compilation uses AArch64 bare-metal target compiler (aarch64-none-elf), from ARM.
Since the development platform that I am using is Windows, I have
installed WSL (Ubuntu) and am using batch scripts to transfer the
necessary files between the current folder and the build folder.

## Running the Image on Raspberry PI 3A+
You will need a SD card with the bare minimum firmware on it. Copy
the kernel8.img file into the root folder of the SD card.

Firmware available here: https://github.com/raspberrypi/firmware/tree/master/boot

Ensure config.txt has the following (if it does not exist then create it):
```
kernel_old=1
disable_commandline_tags=1
disable_overscan=1
framebuffer_swap=0
```

If any problems occur, then it may be an issue with the hardware or power supply.

## Completed Feataures
* Basic GPIO Management (Output)
* Basic GPIO Management (Input)
* Writing to Physical Display - Graphics Management
* Writing PSF2 Fonts to Physical Display - Graphics Management
* System Timer Counter reading - although measurements of time (e.g. seconds ) are not accurate
* Stacktracking and crash() function to dump stack trace (QEMU only)
* DMA & Graphics Doublebuffering (QEMU works but hardware crashing)
  - disabled for now

## Milestones
Current outlook of objectives (may be subject to change)

* Reading Joystick Values
* Generating Interrupt via Timer.
* Creating Task System - OS Programming
