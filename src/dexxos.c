// dexxOS
#include "dexxos.h"

#include "drivers/terminal.h"
#include "drivers/string.h"
#include "drivers/system/memory.h"
#include "drivers/system/heap.h"
#include "drivers/system/paging.h"

#include "drivers/terminal.c"
#include "drivers/string.c"
#include "drivers/system/memory.c"
#include "drivers/system/heap.c"
#include "drivers/system/paging.c"

// create a page for the kernel
static struct paging_4gb_chunk* kernel_chunk = 0;

// Entry point for dexxos
void dexxos(){
    // Initialize text mode
        terminal_initialize();
    // Initialize the heap
        kheap_init();

    // Setup paging
        // Create kernel page
            kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
        // Switch to kernel page
            paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));
        // Enable paging
            paging_enable();

    // dexxOS message
        print_color = c_cyan;
        print("dexxOS\n");
        print_color = c_white;
        print("Hello werido\n");
    // Heap test
        // Allocate and free memory
            void* kptr = kmalloc(50);
            void* kptr2 = kmalloc(5000);
            void* kptr3 = kmalloc(5600);
            kfree(kptr);
            void* kptr4 = kmalloc(50);
        // Free allocated memory
            kfree(kptr2);
            kfree(kptr3);
            kfree(kptr4);
    // Paging test
        // Create a test page memory
            char* test_page = kzalloc(4096);
        // Map are test_page (virtual memory) to (physical memory) 0x1000
            paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)test_page | PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
        // Now modify are phusical memory
            char* ptr2 = (char*)0x1000;
            ptr2[0] = 'A';
            ptr2[1] = 'B';
        // Print to show that the test_page was also changed
            print(ptr2);
            print(" - ");
            print(test_page);
            print("\n");
        // Now modify test_page memory
            test_page[2] = 'C';
        // Show both memorys were changed
            print(ptr2);
            print(" - ");
            print(test_page);
        // Free page
            kfree(test_page);

    // end process
        system_stop();
}