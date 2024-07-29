// dexxOS
#ifndef DEXXOS_H
#define DEXXOS_H

#define bool int
#define true 1
#define false 0

typedef unsigned char byte_t, uint8_t;
typedef unsigned short word_t, uint16_t;
typedef unsigned int dword_t, uint32_t, size_t;
typedef unsigned long qword_t, uint64_t;

// Halts the system (Infinite loop)
extern void system_stop();

// Entry point for dexxos
void dexxos();

#endif