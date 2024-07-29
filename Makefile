all: run

# -------- [ COMMANDS ] ---------
.PHONY: build run clear

build:
# create bin folder or clear all files in it (if exitst)
	mkdir -p bin
	rm -f bin/*
# Compile the bootloader
	nasm -f bin src/bootloader/bootloader.asm -o bin/bootloader.bin
# this assembly is not compiled as bin but elf32 so we can have are meta-data
	nasm -f elf32 src/assembly/dexxos.asm -o bin/dexxos_asm.bin
# compile dexxos_c.bin
# -m32 = 32-bit mode, -ffreestanding = no c libraryes
	gcc -m32 -c -fno-stack-protector -ffreestanding -fno-PIC -std=gnu99 src/dexxos.c -o bin/dexxos_c.bin
# compile dexxos.bin with ( dexxos_asm.bin, dexxos_c.bin )
# linking them together, basicly combined them together
# allow the linker to go into these files and find all the refrences to methods, and give them concreate memory address so that we can call functions from other .o files
# like the extern dexxos lets us call the method from a .c compiled code
	ld -m elf_i386 --oformat=binary -T src/linker.ld -o bin/dexxos.bin bin/dexxos_asm.bin bin/dexxos_c.bin
# Create image
	dd if=/dev/zero of=dexxos.img bs=512 count=60
	dd if=bin/bootloader.bin of=dexxos.img conv=notrunc seek=0
	dd if=bin/dexxos.bin of=dexxos.img conv=notrunc seek=1

run: build
	qemu-system-x86_64 -hda dexxos.img

clear:
	rm -rd bin
	rm -rf dexxos.img
