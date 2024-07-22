#include "dexxos.h"
#include <stddef.h> // size_t
#include <stdint.h> // uint16_t

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;
char print_color = 13; // 1 blue, 2 green, 3 light blue, 4 red, 5 purple, ... , 15 white

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
// returns the size of a string
size_t strlen(const char* str){
    size_t len = 0;
    while(str[len]){
        len++;
    }
    return len;
}
// prints a string to terminal
void print(const char* str){
    size_t len = strlen(str);
    for(int i = 0; i < len; i++){
        terminal_writechar(str[i], print_color);
    }
}
void dexxos()
{
    terminal_initialize();
    print_color = 3; // blue
    print("dexxOS\n");
    print_color = 15; // white
    print("Hello werido");

    while(0 == 0){}

}