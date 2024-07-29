#include "terminal.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

// Create a text/color 16 bit value (switching for little indian)
uint16_t terminal_make_char(char c, char color){
    return (color << 8) | c;
}
// places a char of color to a given x,y text grid slot
void terminal_putchar(int x, int y, char c, char color){
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}
// Writes to terminal and increment position
void terminal_writechar(char c, char color){
    // newLine
    if(c == '\n'){
        terminal_row += 1;
        terminal_col = 0;
        return;
    }
    terminal_putchar(terminal_col,terminal_row,c,color);
    terminal_col += 1;
    if(terminal_col >= VGA_WIDTH){
        terminal_col = 0;
        terminal_row += 1;
    }
}
// clears the text video memory
void terminal_initialize(){
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for(int y = 0; y < VGA_HEIGHT; y++){
        for(int x = 0; x < VGA_WIDTH; x++){
            terminal_putchar(x,y,' ',0);
        }
    }
}
// prints a string to terminal
void print(const char* str){
    size_t len = strlen(str);
    for(int i = 0; i < len; i++){
        terminal_writechar(str[i], print_color);
    }
}