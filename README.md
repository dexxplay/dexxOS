# dexxOS
dexxos is currently a x86 32-bit operating system written from scrach.
The goal is a fast and low on system resources OS, while still being visualy appealing, custimizable, user friendly and powerful.
The os will be modular which means that it and all its features are separate parts working together. so that a person can add or remove what they want. the os will consist of a kernel that will deal with memory managment and basic low-level functions that the OS's apps can call to perform there desired tasks like graphics, audio and more.
**all versions of dexxOS will be tested on real hardwear because thats the point**

## Current-State
 ![Current-State](/images/dexxOS_current.png)
 + bootloader
   + Setup GDT (flat memory model) 
   + Switch To Protction Mode (32-bit)
   + Enabling the A20 line
   + 32-bit Cross Compiler (so we can code in C)
   + Create linker script
   + Small ATA disk driver (loading kernel)
 + Kernel

## End-Goal
 ![End-Goal](/images/dexxOS_goal.png)
### planned Begining App's
 **[Desktop]** - displays the background image and allows positioning app's and folder's within the "Desktop" folder (without you get a black back)<br>
 **[menubar]** - A basic button at the bottom of the screen that displays the logo and when clicked a menu of tabs to launch applications<br>
 **[About]** - displays the operating system version (and other OS version info) (apps can check this version to verify they can run) (mainly for outdated versions)<br>
 **[Terminal]** - a text window for typing commands to directly use your pc (without GUI apps)<br>
 **[System Monitor]** displays memory and active processes, also allowling you to end them<br>
 **[Files]** - visualy view files and create/edit/delete/move/rename them<br>
 **[Viewer]** - image viewer<br>
 **[Calculator]** - calculate numbers (+-×÷)<br>
 **[Defragmenter]** - when ever you write a file to disk the files memory may not be next to eachother but scattered into multiple chunks this is called fragment data. This app defragments the data combining them all in 1 consecutive memory (increasing reading speed)<br>

### Requirments (Linux)
 - sudo apt install `make` (Build tool)
 - sudo apt install `nasm` (Assembler)
 - sudo apt install `qemu-system-x86` (Emulator)
 - sudo apt install `gcc` (Compiling C code)

### How to build
 - Install Requirments
 - Download the project
 - goto root diretory with make file
 - run `make run` to build and run dexxOS
 - run `make clear` to remove all pre-built binarys for clean build
 - run `make` to just build without running