#include <stddef.h>
#include <stdint.h>

#include <io/uart.h>
#include <interrupts.h>
#include <io/gpu.h>



void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{   
    (void) r0;
    (void) r1;
    (void) atags;
    // HDMI
    mem_init(((atag_t *)atags));
    gpu_init();
    
    while (1);
}
