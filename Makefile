CFILES = $(wildcard *.c)
OFILES = $(patsubst %.c,bin/%.o,$(notdir $(CFILES)))
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCINSTALL = ../gcc-arm-10.3-2021.07-mingw-w64-i686-aarch64-none-elf/bin

all: clean kernel8.img

boot.o: boot.s
	$(GCCINSTALL)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot.s -o bin/boot.o

bin/%.o: %.c
	$(GCCINSTALL)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(GCCINSTALL)/aarch64-none-elf-ld -nostdlib bin/boot.o $(OFILES) -T link.ld -o bin/kernel8.elf
	$(GCCINSTALL)/aarch64-none-elf-objcopy -O binary bin/kernel8.elf bin/kernel8.img

clean:
	rm kernel8.elf bin/*.o bin/*.img > /dev/null || true
