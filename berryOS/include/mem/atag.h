/**
 * This library contains all the atags options.
 * Functions will be added as needed.
 * 
 * Atags structs obtained in 
 * http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html#appendix_tag_reference
 * 
 * Used jsandler tutorial as example
 * https://jsandler18.github.io/tutorial/wrangling-mem.html
 * 
 * Raúl Sánchez Montaño
 * 
 * Dic, 3, 2019
*/



#ifndef ATAG_H
#define ATAG_H

#include <stdint.h>
//The memory addresses that contains the data atags.
typedef enum{
	NONE =		0x00000000, //This is the end of the atags list.
	CORE =		0x54410001,
	MEM =		0x54410002,
	VIDEOTEXT =	0X54410003,
	RAMDISK =	0x54410004,
	INITRD2 =	0x54420005,
	SERIALNR =	0x54410006,
	REVISION =	0x54410007,
	VIDEOLFB =	0x54410008,
	CMDLINE =	0x54410009
} atag_tag_t;

//Start of the atags list.
typedef struct atag_core {
        uint32_t flags;              /* bit 0 = read-only */
        uint32_t pagesize;           /* systems page size (usually 4k) */
        uint32_t rootdev;            /* root device number */
} core_t;

//Info about the usable physic memory
typedef struct atag_mem {
        uint32_t size;   /* size of the area */
        uint32_t start;  /* physical start address */
} mem_t;

//Describe VGA text type displays 
typedef struct atag_videotext {
        uint8_t		x;           /* width of display */
        uint8_t		y;           /* height of display */
        uint16_t	video_page;
        uint8_t		video_mode;
        uint8_t		video_cols;
        uint16_t	video_ega_bx;
        uint8_t		video_lines;
        uint8_t		video_isvga;
        uint16_t	video_points;
} videotext_t;

//How the ramdisk will be used by the kernel
typedef struct atag_ramdisk {
        uint32_t flags;      /* bit 0 = load, bit 1 = prompt */
        uint32_t size;       /* decompressed ramdisk size in _kilo_ bytes */
        uint32_t start;      /* starting block of floppy-based RAM disk image */
} ramdisk_t;

//Describes the physical location of the compressed ramdisk image
typedef struct atag_initrd2 {
        uint32_t start;      /* physical start address */
        uint32_t size;       /* size of compressed ramdisk image in bytes */
} initrd2_t;

//64 bit serial number of the board
typedef struct atag_serialnr {
	uint32_t low;
	uint32_t high;
} serialnr_t;

//Board revision
typedef struct atag_revision {
        uint32_t rev;
} revision_t;

//Describes parameters for a framebuffer type display
typedef struct atag_videolfb {
        uint16_t	lfb_width;
        uint16_t	lfb_height;
        uint16_t	lfb_depth;
        uint16_t	lfb_linelength;
        uint32_t	lfb_base;
        uint32_t	lfb_size;
        uint8_t     red_size;
        uint8_t	    red_pos;
        uint8_t	    green_size;
        uint8_t	    green_pos;
        uint8_t	    blue_size;
        uint8_t	    blue_pos;
        uint8_t	    rsvd_size;
        uint8_t	    rsvd_pos;
} videolfb_t;

//Used to pass the commandline to the kernel
typedef struct atag_cmdline {
        char cmdline[1];     /* this is the minimum size */
} cmdline_t;

typedef struct atag {
	uint32_t tag_size;
	atag_tag_t tag;
	union{
		mem_t 		mem;
		core_t 		core;
		videotext_t videotext;
		ramdisk_t 	ramdisk;
		initrd2_t 	initrd2;
		serialnr_t	serial_nr;
		revision_t	revision;
		videolfb_t	videolfb;
		cmdline_t	cmdline;
	};
} atag_t;

/**
 * Return the size of the RAM
 * 
 * @param tag The atags that contains the info
 * 
 * @return The size of the RAM
 *
 */ 
uint32_t get_mem_size(atag_t * tag);

#endif
