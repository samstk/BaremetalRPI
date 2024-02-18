:: VSCode seems to have trouble with intellisense in WSL
:: directory, so this is just a workaround for it,
:: which works well for small applications.

:: Copy all src and include files to build directory
:: (must create directory using WSL, in here samst is username)
@echo off


del /s /f /q "\\wsl.localhost\Ubuntu\home\samst\build-kernel8"
xcopy /Y /E /I "src" "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\src"
xcopy /Y /E /I "include" "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\include"
copy "Makefile" "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\Makefile"
copy "link.ld" "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\link.ld"

: Copy play.sh for debug reasons
copy "play.sh" "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\play.sh"

:: Build kernel8.img using Makefile
bash -c "cd ~/build-kernel8 && chmod +x ./play.sh && make" 

:: Copy output if any
xcopy /Y /E /I  "\\wsl.localhost\Ubuntu\home\samst\build-kernel8\build" "build"

:: Play
"C:\Program Files\qemu\qemu-system-aarch64.exe"  -M raspi3ap -kernel build/kernel8.img -serial stdio -d trace:bcm2837_systmr*,int,in_asm -D qemu.log
exit