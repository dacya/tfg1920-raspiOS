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

typedef struct filesystem_methods{
    int (*write)(char* path, char* text);
    char* (*read)(char* path, uint32_t bytes);
    void (*delete)(char* path);
    void (*createFile)(char* path);
    void (*createDir)(char* path);
    int (*exists)(char* path);
    void (*printFs)(void);
    int (*getFileSize)(char* path);
    void (*changeDir)(char* path);
    void (*listDirectory)(char* path);
} fs_interface;

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
    uint32_t num_childs; //This is not exactly like ext2, but makes it easier some operations 
    dir_entry_t child[MAXFILESPERDIR]; //child[0] --> itself(.) | child[1] --> father(..)
} dir_t;
 
/*
 * Path, directory and file names must be finished in '\0' character.
 */

/**
 * Initialize root directory and the i-node structure
 */
void fs_init(void);

/* *
 * Returns an interface. The user can create his own methods and 
 * initialize the interface with them in fs_init. These methods must:
 *      -> Return the same type that the original methods
 *      -> Have the same parameters
 * 
 */
fs_interface* getFsInterface(void);

/**
 * Check if a file exists.  
 * 
 * @param path The name of the path to check.
 * 
 * @return Returns 1 if path exists, 0 if not.
 */
int exists(char* path);

/**
 * Create a file. This method takes no effect on the following cases:
 *      -> There are no free inodes.
 *      -> File already exists
 *      -> Path is not valid
 *      -> Limit of files per directory reached
 *      -> There are no free memory pages
 * 
 * @param path The path where we want to create the file. The last part of the path is the filename.
 */
void createFile(char* path);

/**
 * Create a directory. This method takes no effect on the following cases:
 *      -> There are no free inodes.
 *      -> File already exists
 *      -> Path is not valid
 *      -> Limit of files per directory reached
 *      -> There are no free memory pages
 * 
 * @param path The path where we want to create the dir. The last part of the path is the dirname.
 */
void createDir(char* path);

/**
 * Obtain the size of a file.  
 * 
 * @param path The filename whose size we want.
 * 
 * @return Returns the size of filename. Returns -1 if the file doesn't exist, is a directory or the path is wrong.
 */
int getFileSize(char* path);

/**
 * Read the content of a file. 
 * 
 * @param path The name of the file we want to read.
 * @param bytes Number of bytes to read.
 * 
 * @return Returns a pointer to char array, whose size is the minimum between bytes and the file's size.
 *         Returns NULL if the file doesn't exist, is a directory or path is wrong.
 *         The user must free this pointer.
 */
char* read(char* path, uint32_t bytes);

/**
 * Concatenate text at the end of the file.  
 * 
 * @param path The name of the file where we want to write.
 * @param text The text to concatenate.
 * 
 * @return Returns the number of written bytes. Returns -1 if the file doesn't exist, is a directory or path is wrong.
 */
int write(char* path, char* text);

/**
 * Delete a file or a directory. If the file is a directory,this method will recursively delete it's 
 * content. If the file doesn't exist, this method takes no effect.
 * 
 * @param path The name of the file we want to delete.
 */
void delete(char* path);

/**
 * Print the whole filesystem recursively. 
 */
void printFs(void);

/**
 * Change the current directory. Root is the first current directory. If the file doesn't exits, is
 * not a directory or path is wrong, this method takes no effect.
 * 
 * @param path The name of the directory.
 */
void changeDir(char* path);


/**
 * List all the files contained in the directory. Files will be printed in white and directories
 * will be printed in blue.
 *   
 * @param path The name of the directory.
 */
void listDirectory(char* path);


#endif
