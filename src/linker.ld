OUTPUT_FORMAT(binary)
ENTRY(start)
SECTIONS
 {
# Tell linker are kernel will be loaded in RAM at memory address 10000
   . = 0x10000;
 }
