CFILES = $(wildcard src/*.c)
OFILES = $(patsubst src/%.c,bin/%.o,$(CFILES))
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
GCCINSTALL = ../gcc-arm-10.3-2021.07-mingw-w64-i686-aarch64-none-elf/bin

all: clean kernel8.img

boot.o: src/boot.s
	$(GCCINSTALL)/aarch64-none-elf-gcc $(GCCFLAGS) -c src/boot.s -o bin/boot.o

bin/%.o: src/%.c
	$(GCCINSTALL)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

font_psf.o: lib/font.psf
	$(GCCINSTALL)/aarch64-none-elf-ld -r -b binary -o bin/font_psf.o lib/font.psf

font_sfn.o: lib/font.sfn
	$(GCCINSTALL)/aarch64-none-elf-ld -r -b binary -o bin/font_sfn.o lib/font.sfn

kernel8.img: boot.o font_sfn.o font_psf.o $(OFILES)
	$(GCCINSTALL)/aarch64-none-elf-ld -nostdlib bin/boot.o bin/font_psf.o bin/font_sfn.o $(OFILES) -T src/link.ld -o bin/kernel8.elf
	$(GCCINSTALL)/aarch64-none-elf-objcopy -O binary bin/kernel8.elf bin/kernel8.img

clean:
	rm bin/kernel8.elf bin/*.o bin/*.img > /dev/null || true
