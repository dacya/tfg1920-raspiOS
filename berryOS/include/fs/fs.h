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
 
/*
 * File and directory names must be finished in '\0' character.
 */

/**
 * Initialize root directory and the i-node structure
 */
void fs_init(void);

/**
 * Check if a file exists.  
 * 
 * @param file The name of the file to check.
 * 
 * @return Returns 1 if file exists, 0 if not.
 */
int exists(char* file);

/**
 * Create a file. This method takes no effect on the following cases:
 *      -> There are no free inodes.
 *      -> File already exists
 *      -> Limit of files per directory reached
 *      -> There are no free memory pages
 * 
 * @param file The name of the file we want to create.
 * @param fnsize The filename's size
 */
void createFile(char* file, int fnsize);

/**
 * Create a directory. This method takes no effect on the following cases:
 *      -> There are no free inodes.
 *      -> File already exists
 *      -> Limit of files per directory reached
 *      -> There are no free memory pages
 * 
 * @param file The name of the direc we want to create.
 * @param fnsize The filename's size
 */
void createDir(char* file, int fnsize);

/**
 * Obtain the size of a file.  
 * 
 * @param filename The filename whose size we want.
 * 
 * @return Returns the size of filename. Returns -1 if the file doesn't exist or is a directory.
 */
int getFileSize(char* filename);

/**
 * Read the content of a file. 
 * 
 * @param filename The name of the file we want to read.
 * @param bytes Number of bytes to read.
 * 
 * @return Returns a pointer to char array, whose size is the minimum between bytes and the file's size.
 *         Returns NULL if the file doesn't exist or is a directory.
 *         The user must free this pointer.
 */
char* read(char* filename, uint32_t bytes);

/**
 * Concatenate text at the end of the file.  
 * 
 * @param filename The name of the file where we want to write.
 * @param text The text to concatenate.
 * 
 * @return Returns the number of written bytes. Returns -1 if the file doesn't exist or is a directory.
 */
int write(char* filename, char* text);

/**
 * Delete a file or a directory. If the file is a directory,this method will recursively delete it's 
 * content. If the file doesn't exist, this method takes no effect.
 * 
 * @param filename The name of the file we want to delete.
 */
void delete(char* filename);

/**
 * Print the whole filesystem recursively. 
 */
void printFs();

/**
 * Change the current directory. Root is the first current directory. If the file doesn't exits or is
 * not a directory, this method takes no effect.
 * 
 * @param filename The name of the directory.
 */
void changeDir(char* filename);


#endif
