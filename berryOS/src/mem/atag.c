/**
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic 16, 2019
 */

#include <mem/atag.h>
#include <io/uart.h>
#include <stdint.h>

#define SIMULATOR 1 //este valor deberá ser 0 si se ejecuta en hardware  

 
uint32_t get_mem_size(atag_t * tag){
	if(SIMULATOR){
                return 1024 * 1024 * 256;       
        }
        while(tag->tag != NONE){
                if(tag->tag == MEM){
                        return tag->mem.size;
                }
                tag = ((uint32_t *) tag) + tag->tag_size;
        }
        return 0;
}

