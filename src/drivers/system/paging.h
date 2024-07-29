// paging
#ifndef PAGING_H
#define PAGING_H

#define PAGING_CACHE_DISABLED  0b00010000
#define PAGING_WRITE_THROUGH   0b00001000
#define PAGING_ACCESS_FROM_ALL 0b00000100
#define PAGING_IS_WRITEABLE    0b00000010
#define PAGING_IS_PRESENT      0b00000001

#define PAGING_TOTAL_ENTRIES_PER_TABLE 1024
#define PAGING_PAGE_SIZE 4096

struct paging_4gb_chunk{
    uint32_t* directory_entry;
};
// Creates new 4GB chunk
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags);
// Switch current page
void paging_switch(uint32_t* directory);
// Set paging to be used by CPU
void paging_enable();

// Returns a directory from a paging_4gb_chunk
uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk* chunk);

// Set a page_table_entry to the value given
int paging_set(uint32_t* directory, void* virt, uint32_t val);

#endif