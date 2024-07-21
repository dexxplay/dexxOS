# -------- [ COMMANDS ] ---------
.PHONY: build run clear

build:
	nasm -f bin src/boot/boot.asm -o bin/boot.bin

run: build
	qemu-system-x86_64 -hda bin/boot.bin

clear:
	rm -rf bin/boot.bin