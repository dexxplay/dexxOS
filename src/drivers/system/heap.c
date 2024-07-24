#include "heap.h"
#include "memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

// Setup the kernel heap for use
void kheap_init(){
    // Set up kernel_heap_table
        kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*) HEAP_TABLE_ADDRESS;
        kernel_heap_table.total = HEAP_SIZE_BYTES / HEAP_BLOCK_SIZE; // total_table_entries
    // get the end of are table
        void* end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
        void* start = (void*)HEAP_ADDRESS;
    // Check if the start and end address are aligned with are block size (divides evenly)
        if(!((((unsigned int)start + (unsigned int)end) % HEAP_BLOCK_SIZE) == 0)){
            print_color = c_lred;
            print("heap::kheap_init - not aligned");
            system_stop();
        }
    // Set kheap variables 
        kernel_heap.start = start;
        kernel_heap.table = &kernel_heap_table;
    // Validate heap table is the right size to hold all heap blocks states
        size_t table_size = (size_t)(end - start);
        size_t total_blocks = table_size / HEAP_BLOCK_SIZE;
        if(kernel_heap_table.total != total_blocks){
            print_color = c_lred;
            print("heap::kheap_init - Table size incorrect");
            system_stop();
        }
    // Initulize all the blocks in the heap table to free
        table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * kernel_heap_table.total;
        memset(kernel_heap_table.entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);
}
// Search the heap for a free space of size total_blocks and return the starting address
int heap_get_start_block(struct heap* heap, uint32_t total_blocks){
    // Set up variables
        struct heap_table* table = heap->table;
        int bc = 0;  // current block
        int bs = -1; // start block
    // Loop throw entry block table searching for free memory
        for(size_t i = 0; i < table->total; i++){
            // if entry we are on is not free, reset values and restart
            if(table->entries[i] & 0x0f != HEAP_BLOCK_TABLE_ENTRY_FREE){
                bc = 0;
                bs = -1;
                continue; // go back to start of loop
            }
            // if this is the first block (set it)
            if(bs == -1){
                bs = i;
            }
            bc++;
            // if bc equals are total_blocks we have found a start block with enough free blocks
            if(bc == total_blocks){
                break;
            }
        }
    // Finished searching return result
        if(bs == -1){
            return -1; // No memory
        }
        return bs;
}
// make a found starting address of total_blocks as used
void heap_mark_blocks_taken(struct heap* heap, int start_block, int total_blocks){
    // get heap end block index
    int end_block = (start_block + total_blocks)-1;
    // if there is more then one block set the (has next bit)
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks > 1){
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    // loop throw block enties updating there bit states
    for(int i = start_block; i <= end_block; i++){
        heap->table->entries[i] = entry; // set entry var to real block entry
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN; // reset entry to taken
        // if this is not end_block set next block as (has next)
        if(i != end_block -1){
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

// allocate memory from the kernel heap
void* kmalloc(size_t size){
    // Calculate how many blocks of memory are needed for given size
    uint32_t total_blocks = (size / HEAP_BLOCK_SIZE) + (size % HEAP_BLOCK_SIZE > 0 ? 1 : 0);
    void* address = 0;
    // find a block that has enough free blocks in a row for are allication
    int start_block = heap_get_start_block(&kernel_heap, total_blocks);
    if(start_block < 0){
        goto out;
    }
    // gets the address to return for use
    address = kernel_heap.start + (start_block * HEAP_BLOCK_SIZE);
    // Mark the blocks as taken
    heap_mark_blocks_taken(&kernel_heap, start_block, total_blocks);

out:
    return address;
}
// free allocateted heap memory
void kfree(void* ptr){
    // Calculate starting block
        int starting_block = ((int)(ptr - kernel_heap.start)) / HEAP_BLOCK_SIZE;
    // Loop throw the kernel heap at are starting blovk freeing each entry tell we reach end
    struct heap_table* table = kernel_heap.table;
    for(int i = starting_block; i < (int)table->total; i++){
        HEAP_BLOCK_TABLE_ENTRY entry = table->entries[i];
        table->entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        // check if there is another block if not end
        if(!(entry & HEAP_BLOCK_HAS_NEXT)){
            break;
        }
    }
}