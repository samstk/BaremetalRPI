# BaremetalRPI
A C++ implementation of a bare metal OS for the Raspberry PI 3a+.

## Credits
Credits to bztsrc on Github for his C tutorials
(https://github.com/bztsrc/raspi3-tutorial/blob/master/), which helped enormously for the
initial setup (mainly start.S and link.ld). It is another helpful resource which is worth
a look at.

Also credits to PeterLemon for his assembly tutorials which also helped enormously: https://github.com/PeterLemon/RaspberryPi/blob/master

## Why C++ and not C
Previously I have programmed a bare metal RPI image using C, however
have decided to go with C++ due to a few reasons (mainly preference):
* C++ has a number of features such as encapsulation (writing methods inside structs/classes) that make it desirable. This OOP way makes it easier to write 'cleaner' code.
* C++ supports everything C can, so there shouldn't be any issues with compatibility.

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

## Milestones
Current outlook of objectives (may be subject to change)
* Basic GPIO Management (Output) - Completed (Bug may exist)
* Basic GPIO Management (Input) - Completed
* Writing to Physical Display - Graphics Management (Complete)
* Writing fonts to Physical Display - Graphics Management (Complete - ASCII PSF2 only)
* System Timer Counter - Completed
* Generating Interrupt via Timer.
* Creating Task System - OS Programming
