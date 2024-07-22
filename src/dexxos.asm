; dexxOS

bits 32
; these globals make these functions callable in c
; this only works when its compiled with -f elf32, NOT -f bin. (it needs the linker to work all this out)
global start
global call
; this allows us to point to a function with this name in a .c compiled file
extern dexxos ; defined in dexxos.c (the linker resoves the memory address so we can jump to it)

start:
 call dexxos
 
 halt:
  hlt
 jmp halt