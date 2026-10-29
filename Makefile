CC = gcc -m32 -O2 -ffreestanding \
	-no-pie -fno-pie -fno-stack-protector -mno-sse
# -std=c99/gnu99
AS = nasm -f elf
LD = ld -m elf_i386 -T linker-script.ld
OBJCOPY = objcopy -I elf32-i386 -O binary

QEMU = qemu-system-i386 -cpu pentium2 -m 1g -device VGA

# collect the byte counts of all present .o files, return the last (total) one
OBJECT_BYTES = wc --bytes *.o \
	| cut -f1 --whitespace-delimited=trimmed \
	| tail -n 1


.PHONY: all clean build test debug
all: clean build test

clean:
	rm -f *.o
	rm -f *.bin

build: clean
	$(CC) -c vedro/*.c
	$(AS)    vedro/*.asm

	$(AS)    boot-sector.asm -o boot-sector.o -dN=$$($(OBJECT_BYTES))

	$(LD) *.o -o elf.bin
	$(OBJCOPY) elf.bin img.bin
#TODO assert the generated bin is actually under that ^^ OBJECT_BYTES +512 ?

test: build
	$(QEMU) -fda img.bin

debug: build
	$(QEMU) -fda img.bin -s -S #&
#		-monitor stdio
#	gdb
