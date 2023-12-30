# Windows subsystem for linux has been used to compile this makefile.

SRCS = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)
INCLUDEARGS = -I include/
CFLAGS = -Wall -Wno-unknown-pragmas -fno-exceptions -ffreestanding -O3 -nostdinc -nostdlib -nostartfiles -mcpu=cortex-a53+nosimd
AARCH64PATH = ~/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/bin/aarch64-none-elf

all: clean kernel8.img

%.o: %.cpp
	$(AARCH64PATH)-g++ $(INCLUDEARGS) $(CFLAGS) -c $< -o $@

kernel8.img: $(OBJS)
	# Build Object Files for Preprocessing
	$(AARCH64PATH)-g++ $(INCLUDEARGS) $(CFLAGS) -c src/start.S -o src/start.o
	$(AARCH64PATH)-ld -nostdlib src/start.o $(OBJS) -T link.ld -o build/preproc.elf
	# Construct Function Map from Preprocessing
	echo ".section \".data\"" >> build/labels.S
	echo ".global _funcmap" >> build/labels.S
	echo "_funcmap:" >> build/labels.S
	echo "\t.incbin \"build/labels.bin\"" >> build/labels.S
	($(AARCH64PATH)-objdump -D build/preproc.elf | grep ">:") >> build/labels.bin
	$(AARCH64PATH)-g++ $(INCLUDEARGS) $(CFLAGS) -c build/labels.S -o build/labels.o
	# Rebuild funcmap to contain reference to actual function map instead of NULL
	$(AARCH64PATH)-g++ $(INCLUDEARGS) $(CFLAGS) -D POSTPROC -c src/resources/funcmap.cpp -o src/resources/funcmap.o
	# Build assembly
	$(AARCH64PATH)-ld -nostdlib src/start.o $(OBJS) build/labels.o -T link.ld -o build/kernel8.elf
	$(AARCH64PATH)-objcopy -O binary build/kernel8.elf build/kernel8.img
	$(AARCH64PATH)-objdump -D build/kernel8.elf >> build/kernel8.dis

clean:
	find . -type f -name "*.o" -delete
	rm build/kernel8.elf >/dev/null 2>/dev/null || true