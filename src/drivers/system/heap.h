// heap
#ifndef HEAP_H
#define HEAP_H

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00
#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST 0b01000000

// later we will determin the ram on the system instead of a static value
#define HEAP_SIZE_BYTES 104857600 // 100MB heap size
#define HEAP_BLOCK_SIZE 4096
#define HEAP_ADDRESS 0x01000000
#define HEAP_TABLE_ADDRESS 0x00007E00

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;
// The table of flags that define the stats of are heap blocks (free/used)
struct heap_table{
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;
};
// Heap data pool
struct heap{
    struct heap_table* table;
    // Start address of the heap data pool
    void* start;
};

// Setup the kernel heap for use
void kheap_init();
// allocate memory from the kernel heap
void* kmalloc(size_t size);
// allocate memory kernel heap and clears with zeros
void* kzalloc(size_t size);
// free allocateted heap memory
void kfree(void* ptr);

#endif