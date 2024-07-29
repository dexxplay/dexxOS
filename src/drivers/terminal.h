// textMode
#ifndef TEXTMODE_H
#define TEXTMODE_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

// Colors
#define c_black 0
#define c_blue 1
#define c_green 2
#define c_cyan 3
#define c_red 4
#define c_purple 5
#define c_brown 6
#define c_lgray 7
#define c_dgray 8
#define c_lblue 9
#define c_lgreen 10
#define c_lcyan 11
#define c_lred 12
#define c_lpurple 13
#define c_yellow 14
#define c_white 15
// The color the print function uses
char print_color = 13;
// prints a string to terminal
void print(const char* str);

#endif