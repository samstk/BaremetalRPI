qemu-system-aarch64 -M raspi3ap -kernel build/kernel8.img -serial stdio -d trace:bcm2837_systmr*,int,in_asm -D qemu.log
