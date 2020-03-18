/**
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */

#include <mem/mem.h>
#include <mem/atag.h>
#include <io/uart.h>
#include <utils/stdlib.h>
#include <stdint.h>
#include <stddef.h>

extern uint8_t __end; 
static uint32_t num_pages;

IMPLEMENT_LIST(page);

static page_t * all_pages_array;
page_list_t free_pages;

/**
 * Heap
 */

static void heap_init(uint32_t heap_start);
/**
 * impliment kmalloc as a linked list of allocated segments.
 * Segments should be 4 byte aligned.
 * Use best fit algorithm to find an allocation
 */


static heap_segment_t * heap_segment_list_head;


void mem_init(atag_t * atags) {
    uint32_t mem_size, page_array_len, kernel_pages, i;
    
    //get the total number of pages
    mem_size = get_mem_size(atags);
    num_pages = mem_size / PAGE_SIZE;

    // Allocate space for all those pages' metadata.  Start this block just after the kernel image is finished
    page_array_len = sizeof(page_t) * num_pages;
    all_pages_array = (page_t *)&__end;
    bzero2(all_pages_array, page_array_len);
    INITIALIZE_LIST(free_pages, page);

    // Iterate over all pages and mark them with the appropriate flags
    // Start with kernel pages
    kernel_pages = ((uint32_t)&__end) / PAGE_SIZE;
    for (i = 0; i < kernel_pages; i++) {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_page = 1;
    }

     // Reserve 1 MB for the kernel heap
    for (; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++) {
        all_pages_array[i].vaddr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_heap_page = 1;
    }

    // Map the rest of the pages as unallocated, and add them to the free list
    for (; i < num_pages; i++) {
        all_pages_array[i].flags.allocated = 0;
        append_page_list(&free_pages, &all_pages_array[i]);
    }

    //Initialize the heap
    uint32_t page_array_end = ((uint32_t)&__end) + page_array_len;
    heap_init(page_array_end);
}

static void heap_init(uint32_t heap_start) {
    heap_segment_list_head = (heap_segment_t *) heap_start;
    bzero2(heap_segment_list_head, sizeof(heap_segment_t));
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
    heap_segment_list_head->next = NULL;
    heap_segment_list_head->prev = NULL;
}


void * alloc_page(void) {
    page_t * page;
    void * page_mem;

    if(size_page_list(&free_pages) == 0)
        return NULL;
    
    // Get a free page
    page = pop_page_list(&free_pages);
    page->flags.allocated = 1;
    page->flags.kernel_page = 1;

    // Get the address the physical page metadata refers to
    page_mem = (void *)((page - all_pages_array) * PAGE_SIZE);

    // Zero out the page, big security flaw to not do this
    bzero2(page_mem, PAGE_SIZE);
    return page_mem;
}


void free_page(void * ptr) {
    page_t * page;

    // Get page metadata from the physical address
    page = all_pages_array + ((uint32_t)ptr / PAGE_SIZE);

    // Mark the page as free
    page->flags.allocated = 0;
    append_page_list(&free_pages, page);
}

void * kmalloc(uint32_t bytes) {
    heap_segment_t * current, * best = NULL;
    int diff, best_diff = 0x7FFFFFFF;
    
    
    
    // Add the header to the number of bytes we need and make the size 16 byte aligned
    bytes += sizeof(heap_segment_t);
    if(bytes % 16 != 0){
        bytes += 16 - (bytes % 16);
    }
    
    // Find the allocation that is closest in size to this request Could be improved with a divide_and_conquer algorithm?

    for(current = heap_segment_list_head; current != NULL; current = current->next){
        
        diff = current->segment_size - bytes;
        if(diff < best_diff && diff >= 0 && !current->is_allocated){
            best = current;
            best_diff = diff;
        }
    }
    //There is no free memory left.
    if(best == NULL)
        return NULL;
    
    // If the best difference we could come up with was large, split up this segment into two.
    // Since our segment headers are rather large, the criterion for splitting the segment is that
    // when split, the segment not being requested should be twice a header size
    if (best_diff > (int)(2 * sizeof(heap_segment_t))) {
        bzero2(((void*) best) + bytes, sizeof(heap_segment_t));
        current = best->next;

        best->next = ((void*) best) + bytes;
        best->next->next = current;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - bytes;
        best->segment_size = bytes;

        current->prev = best->next;
    }

    best->is_allocated = 1;
    return best + 1; 
}

void kfree(void * ptr) {
    heap_segment_t * seg;

    if (!ptr)
        return;

    seg = ptr - sizeof(heap_segment_t);
    seg->is_allocated = 0;
    while (seg->prev != NULL && seg->prev->is_allocated == 0){
        seg->prev->segment_size += seg->segment_size;
        seg->prev->next = seg->next;
        if(seg->next != NULL)
            seg->next->prev = seg->prev;
        seg = seg->prev;
    }
    
    while (seg->next != NULL && seg->next->is_allocated == 0) {
        if (seg->next->next != NULL)
            seg->next->next->prev = seg;
        seg->segment_size += seg->next->segment_size;
        seg->next = seg->next->next;   
    }        
}

void print_data(void) {
    heap_segment_t * seg = heap_segment_list_head;
    
    while (seg->next != NULL) {
        if (seg->is_allocated == 1) {
            uart_puts("Cadena --> ");
            uint32_t aux = ((uint32_t)seg) + sizeof(heap_segment_t); 
            uart_puts((char *)aux);
            uart_puts("   is_allocated:");
            aux = seg->is_allocated;
            uart_puts(itoa(aux));
            uart_putc('\n');    
        }
        seg = seg->next;
    }
}
