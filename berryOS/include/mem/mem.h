/**
 * This library contains methods for dynamic memory.
 * Also manages the pages. 
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */


#ifndef MEM_H
#define MEM_H


#include <mem/atag.h>
#include <utils/list.h>
#include <stdint.h>
#define PAGE_SIZE (1024 * 4)
#define KERNEL_HEAP_SIZE (1024 * 1024)

//This structs store metadata about the pages 
typedef struct {
    uint8_t allocated: 1;      //this page is allocated
    uint8_t kernel_page: 1;    //this page is part of the kernel
    uint8_t kernel_heap_page: 1;	
    uint32_t reserved: 29;
} page_flags_t;

typedef struct page{
    uint32_t vaddr_mapped;      //the virtual address that maps to this page
    page_flags_t flags;
    DEFINE_LINK(page);
} page_t;

typedef struct heap_segment{
    struct heap_segment * next;
    struct heap_segment * prev;
    uint32_t is_allocated;
    uint32_t segment_size;  // Includes this header
} heap_segment_t; //16 bytes of size

DEFINE_LIST(page);
DEFINE_HEADERS(page);

/**
 * Initialize the memory, using the atags of the boot stage.
 * 
 * @param atags The atags that contains the info.
 * 
 */
void mem_init(atag_t * atags);

/**
 * Reserve a memory page.
 * 
 * @return The memory page address.
 */
 
void * alloc_page(void);

/**
 * Free a memory page
 * 
 * @param ptr The memory page address we want to free.
 */ 
 
void free_page(void * ptr);

/**
 * Reserve <bytes> bytes of data. 
 * 
 * @param bytes The number of bytes we want to reserve.
 */ 
 
void * kmalloc(uint32_t bytes);

/**
 * Free some data.
 * 
 * @param ptr The memory address we want to free.
 */ 
 
void kfree(void *ptr);

void print_data(void);


#endif
