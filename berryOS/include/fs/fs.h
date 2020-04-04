/**
 * 
 * This library contains all needed strutures and methods for
 * filesystem management
 * 
 * Raúl Sánchez Montaño
 * 
 * Apr 1, 2020
 */


#ifndef FS_H
#define FS_H

#include <stdint.h>
#include <utils/list.h>
#include <mem/mem.h> 
#include <math.h>

#define MAXPAGESPERFILE 2
#define MAXFILESIZE (MAXPAGESPERFILE*PAGE_SIZE)  //8 KiB
#define MAXFILENAMESIZE 20 

typedef struct i_node {
    uint32_t size; //In case of a dir, size means num of entries. In case of a file, size means filesize 
    uint8_t num_pages; //Num of pages used
    uint8_t type: 1; //0 --> File | 1 --> Dir
    uint8_t free: 1; //0 --> free | 1 --> not free
    page_t* pages[MAXFILESIZE/PAGE_SIZE]; //Pages used
    //uint8_t mode: 2; // bit 0 --> Read | bit 1 --> write
} i_node_t;

typedef struct dir_entry {
    uint32_t inode_num;
    char filename[MAXFILENAMESIZE];
    uint8_t fn_size;
} dir_entry_t;

#define MAXFILESPERDIR (PAGE_SIZE/sizeof(dir_entry_t))

typedef struct dir {
    char real_name[MAXFILENAMESIZE]; //This is not exactly like ext2, but makes it easier some operations
    uint8_t real_name_size;
    uint32_t num_childs;
    dir_entry_t child[MAXFILESPERDIR]; //child[0] --> itself(.) | child[1] --> father(..)
} dir_t;
 

/**
 * Initialize root directory and all needed
 * structures. 
 */
void fs_init(void);

void createFile(char* file, int fnsize);

int getFileSize(char* filename);

char* read(char* filename, uint32_t bytes);

int write(char* filename, char* text);



#endif
