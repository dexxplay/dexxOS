#include "paging.h"
#include "heap.h"

// Sets the given paging directory to be used by the CPU
void paging_load_directory(uint32_t* directory);
// holds the currently set page directory
static uint32_t* current_directory = 0;

// Creates new 4GB chunk
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags){
    // Create page directory and entrys
    uint32_t* directory = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
    int offset = 0;
    for(int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; i++){
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for(int n = 0; n < PAGING_TOTAL_ENTRIES_PER_TABLE; n++){
            entry[n] = (offset + (n * PAGING_PAGE_SIZE)) | flags;
        }
        offset += (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE);
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITEABLE; // the directory is writable but indivudal pages might not be
    }
    struct paging_4gb_chunk* chunk = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk->directory_entry = directory;
    return chunk;
}
// Switch current page
void paging_switch(uint32_t* directory){
    paging_load_directory(directory);
    current_directory = directory;
}
// Returns a directory from a paging_4gb_chunk
uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk){
    return chunk->directory_entry;
}
// Converts a virtual_address to a dir_index and table_index
int paging_get_indexes(void* virtual_address, uint32_t* dir_index, uint32_t* table_index){
    int res = 0;
    // Check if virtual_address is aligned
    if(((uint32_t)virtual_address % PAGING_PAGE_SIZE) != 0){
        res = -1;
        goto out;
    }
    *dir_index = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *table_index = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);
out:
    return res;
}
// Set a page_table_entry to the value given
int paging_set(uint32_t* directory, void* virt, uint32_t val){
    // Check if virtual_address is aligned
    if(((uint32_t)virt % PAGING_PAGE_SIZE) != 0){
        return -1;
    }
    //
    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    int res = paging_get_indexes(virt,&directory_index,&table_index);
    if(res < 0){
        return res;
    }
    uint32_t entry = directory[directory_index];
    // extract out the address and set it
    uint32_t* table = (uint32_t*)(entry & 0xfffff000);
    table[table_index] = val;
}