/**
 *
 * Raúl Sánchez Montaño
 * 
 * Apr 1, 2020
 */


#include <fs/fs.h>
#include <mem/mem.h>
#include <utils/stdlib.h>
#include <io/uart.h>

#define NUM_PAGES_INODE_TABLE 2
#define NUM_INODES ((PAGE_SIZE/sizeof(i_node_t))*NUM_PAGES_INODE_TABLE) 
#define NUM_INODES_PER_PAGE (NUM_INODES/NUM_PAGES_INODE_TABLE)          

i_node_t* i_node_list_pages[NUM_PAGES_INODE_TABLE];

dir_t* root_dir;
dir_t* current_glob;
int freeInodes;
uint32_t minFreeInode;

fs_interface interface;

/* 
----------------------------------------------------
                Auxiliar functions
----------------------------------------------------
*/

//Calculate the i-node position using the i-node number
static i_node_t* get_inode(uint32_t num){
    return (i_node_t*)(((int)i_node_list_pages[num / NUM_INODES_PER_PAGE]) + (num % NUM_INODES_PER_PAGE)*sizeof(i_node_t)); 
}

//Returns the lowest i-node number. It uses the global variable "minFreeInode" for more efficiency.
static uint32_t getFreeInode(){
    if(freeInodes == 0)
        return 0;
    i_node_t* pointer;
    uint32_t i;
    freeInodes--;
    for(i = minFreeInode; i < NUM_INODES; i++){
        pointer = get_inode(i);
        if(pointer->free == 0){
            uint32_t aux = minFreeInode;
            minFreeInode = i + 1;
            return aux;
        }
    }
    return 0;
}

//Return the file position in the father's child array, or -1 if not found.
//first_child is used to discard "." and ".." when needed.
static int fileExists(char* file, uint32_t first_child, dir_t* father){  //0 if file doesn't exist in the current directory, number of child if exists
    uint32_t i = first_child;
    while(i < father->num_childs){
        if (streq(file,father->child[i].filename))
            return i;
        i++;
    }
    return -1;
}

static dir_t* calculatePath(char* path, char* filename){
    filename[MAXFILENAMESIZE - 1] = '\0';
    dir_t* father = current_glob;
    if(*path == '~'){
        father = root_dir;
        path++;
    }
    int j = 0;
    while(*path != '\0'){
        if(*path == '/'){
            int k;
            filename[j] = '\0';
            if((k = fileExists(filename,0,father)) == -1){
                return NULL;
            }
            father = (dir_t*)get_inode(father->child[k].inode_num)->pages[0];
            j = 0;
        }
        else
            filename[j++] = *path;
        path++;
    }
    filename[j] = '\0';
    return father;
}

int exists(char* path){
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    int aux = fileExists(file, 0, curr_dir) != -1;
    kfree(file); 
    return aux;
}

int getFileSize(char* path){
    int i;
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || (i = fileExists(file, 2, curr_dir)) == -1){
        kfree(file); return -1;
    }
    kfree(file);
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    if(inFile->type == 1)
        return -1;
    return inFile->size;
}

/* 
----------------------------------------------------
            Initialization function
----------------------------------------------------
*/

void fs_init(void){
    
    /*We initialize the interface function.
      User could create his own methods and 
      initialize them. To obtain this interface
      user must call getFsInterface method*/  
    interface.write = write;
    interface.read = read;
    interface.delete = delete;
    interface.createFile = createFile;
    interface.createDir = createDir;
    interface.exists = exists;
    interface.printFs = printFs;
    interface.getFileSize = getFileSize;
    interface.changeDir = changeDir;


    uint32_t i;
    i_node_t* iter;
    
    for(i = 0; i < NUM_PAGES_INODE_TABLE; i++){ // We reserve the num of pages needed
        i_node_list_pages[i] = alloc_page();
    }

    iter = i_node_list_pages[0];
    for (i = 0; i < NUM_INODES; i++){ //We initialize all i-nodes to free
        iter->free = 0;
        iter = get_inode(i+1);
    }

    iter = i_node_list_pages[0];  //Initialize Root inode and create root directory
    iter->type = 1;
    iter->free = 1;
    iter->num_pages = 1;
    iter->size = 2;
    iter->pages[0] = alloc_page();
    root_dir = (dir_t*)iter->pages[0];
    
    root_dir->num_childs = 2;

    memcpy(root_dir->child[0].filename, ".\0", 2);
    root_dir->child[0].fn_size = 2;
    root_dir->child[0].inode_num = 0;
    
    memcpy(root_dir->child[1].filename, "..\0", 3);
    root_dir->child[1].fn_size = 3;
    root_dir->child[1].inode_num = 0;
    
    current_glob = root_dir; //Root is the current directory

    freeInodes = NUM_INODES - 1; //Num of free inodes
    minFreeInode = 1;
}


/* 
----------------------------------------------------
                Management functions
----------------------------------------------------
*/

fs_interface* getFsInterface(void){
    return &interface;
}

void createFile(char* path, int fnsize){
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || freeInodes == 0 || fileExists(file, 0, curr_dir) != -1 || curr_dir->num_childs == MAXFILESPERDIR){
        kfree(file); return;
    }
    
    void* aux = alloc_page();
    if(aux == NULL)
        return;

    uint32_t new = getFreeInode();
    if(new == 0)
        return;

    i_node_t* newPoint = get_inode(new);
    newPoint->pages[0] = aux;
    newPoint->free = 1;
    newPoint->num_pages = 1;
    newPoint->size = 0;
    newPoint->type = 0;
    
    memcpy(curr_dir->child[curr_dir->num_childs].filename, file, MIN(MAXFILENAMESIZE, fnsize));
    curr_dir->child[curr_dir->num_childs].filename[MIN(MAXFILENAMESIZE - 1, fnsize)] = '\0';
    curr_dir->child[curr_dir->num_childs].fn_size = MIN(MAXFILENAMESIZE, fnsize + 1);
    curr_dir->child[curr_dir->num_childs].inode_num = new;
    curr_dir->num_childs++;
    kfree(file);
}

void createDir(char* path, int fnsize){
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || freeInodes == 0 || curr_dir->num_childs == MAXFILESPERDIR || fileExists(file, 0, curr_dir) != -1){
        kfree(file); return;
    }

    dir_t* aux = (dir_t*)alloc_page();
    if(aux == NULL){
        kfree(file); return;
    }
    
    uint32_t new = getFreeInode();
    i_node_t* newPoint = get_inode(new);
    newPoint->pages[0] = (page_t*)aux;
    newPoint->free = 1;
    newPoint->num_pages = 1;
    newPoint->type = 1;
    
    
    aux->num_childs = 2;

    memcpy(aux->child[0].filename, ".\0", 2);
    aux->child[0].fn_size = 2;
    aux->child[0].inode_num = new;
    
    memcpy(aux->child[1].filename, "..\0", 3);
    aux->child[1].fn_size = 3;
    aux->child[1].inode_num = curr_dir->child[0].inode_num;
    
    memcpy(curr_dir->child[curr_dir->num_childs].filename, file, fnsize);
    curr_dir->child[curr_dir->num_childs].fn_size = fnsize;
    curr_dir->child[curr_dir->num_childs++].inode_num = new;
    kfree(file);
    return;
}


int write(char* path, char* text){
    int i;
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || (i = fileExists(file, 2, curr_dir)) == -1){
        kfree(file); return -1;
    }
    kfree(file);

    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    if(inFile->type == 1)
        return -1;

    char* pos = ((char*)inFile->pages[inFile->size/PAGE_SIZE]) + (inFile->size % PAGE_SIZE);
    int actual_page = inFile->size/PAGE_SIZE;
    i = 0;
    while(*text != '\0'){
        if((i + inFile->size) % PAGE_SIZE == 0 && (i + inFile->size) != 0){ //If we reach the endo of the page, we change file
            
            if(actual_page == MAXPAGESPERFILE - 1){ //If file already use all pages allowed, return the written bytes.
                inFile->size += i;
                return i;
            }
            else if(actual_page < inFile->num_pages - 1){ //If we still have unused pages allocated, we use them. 
                pos = (char*)inFile->pages[actual_page + 1];
            }
            else{ //We need to allocate a new page
                void* aux;
                if ((aux = alloc_page()) == NULL){ //If there are no free pages, return the written bytes.
                    inFile->size += i;
                    return i;
                }
                inFile->pages[inFile->num_pages++] = aux;
                pos = aux;
            }
                
        }
        *pos = *text;
        i++;
        text++;
        pos++;
        
    }
    inFile->size += i;
    return i;
}

char* read(char* path, uint32_t bytes){  
    int i;
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || (i = fileExists(file, 2, curr_dir)) == -1){
        kfree(file); return NULL;
    }
    kfree(file);
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    
    if(inFile->type == 1)
        return NULL;
    
    bytes = MIN(bytes, inFile->size);
    char* sol;
    if ((sol = kmalloc(bytes + 1)) == NULL)
        return NULL;

    int j = 0;
    char* pos = (char *)inFile->pages[j++];
    char* aux = sol;

    i = 0;
    while((uint32_t)i < bytes){
        if(i % PAGE_SIZE == 0 && i != 0)
            pos = (char *)inFile->pages[j++];

        *aux = *pos;
        i++;
        aux++;
        pos++;
    }
    sol[bytes] = '\0';
    return sol;

}

void changeDir(char* path){
    int i;
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if (curr_dir == NULL || (i = fileExists(file, 0, curr_dir)) == -1){
        kfree(file); return;
    }
    kfree(file); 
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    
    if(inFile->type == 0)
        return;
    
    current_glob = (dir_t*)inFile->pages[0];
    return;
}

/* 
----------------------------------------------------
                Delete functions
----------------------------------------------------
*/

//Delete file and free all pages used (memory pages, i-node structure and father reference to child)
static void deleteFile(dir_t* father, i_node_t* inFile, uint32_t numChild){
    uint32_t i;
    
    if(father->child[numChild].inode_num < minFreeInode)
        minFreeInode = father->child[numChild].inode_num;
    freeInodes++;    

    for(i = 0; i < inFile->num_pages; i++){
        free_page(inFile->pages[i]);
        inFile->pages[i] = 0;
    }
    inFile->free = 0;
    inFile->size = 0;
    inFile->type = 0;
    
    bzero2(father->child[numChild].filename, father->child[numChild].fn_size);
    father->child[numChild].fn_size = 0;
    father->child[numChild].inode_num = 0;
    father->num_childs--;
    return;
    
}

//Delete recursively all the content in the directory, incluiding its own metadata
static void deleteDirContent(i_node_t* inFile){
    dir_t* dir = (dir_t*)inFile->pages[0];
    i_node_t* aux;

    uint32_t i;
    for(i = 2; i < dir->num_childs; i++){
        aux = get_inode(dir->child[i].inode_num);
        if(aux->type == 0){
            deleteFile(dir, aux, i);
        }
        else{
            deleteDirContent(aux);
        }
    }
    if(dir->child[0].inode_num < minFreeInode)
       minFreeInode = dir->child[0].inode_num;
    freeInodes++;    
    free_page(inFile->pages[0]);
    inFile->pages[0] = 0;
    inFile->free = 0;
    inFile->size = 0;
    inFile->type = 0;
    dir->num_childs = 0;
    return;
}

void delete(char* path){
    int i;
    char* file = kmalloc(MAXFILESIZE);
    dir_t* curr_dir = calculatePath(path, file);
    if(curr_dir == NULL || (i = fileExists(file, 2, curr_dir)) == -1){
        kfree(file); return;
    }
    kfree(file);

    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    if(inFile->type == 0)
        deleteFile(curr_dir, inFile, i);
    else{
        deleteDirContent(inFile);   //We have deleted the dir information, but we still need to delete the father reference to the dir
        bzero2(curr_dir->child[i].filename, (int)curr_dir->child[i].fn_size);
        curr_dir->child[i].fn_size = 0;
        curr_dir->child[i].inode_num = 0;
        curr_dir->num_childs--;
    }

    uint32_t k = curr_dir->num_childs;    

    if((uint32_t)i != k){ //If the file deleted is not the last, we exchange the deleted position with the last position
        memcpy(curr_dir->child[i].filename, curr_dir->child[k].filename, curr_dir->child[k].fn_size);
        curr_dir->child[i].fn_size = curr_dir->child[k].fn_size;
        curr_dir->child[i].inode_num = curr_dir->child[k].inode_num;
        
        bzero2(curr_dir->child[k].filename, curr_dir->child[k].fn_size);
        curr_dir->child[k].fn_size = 0;
        curr_dir->child[k].inode_num = 0;
    }
    return;
}

/* 
----------------------------------------------------
                Print functions
----------------------------------------------------
*/

//This method print recursively from inode.
//j is used to count the level for print spaces.
static void recPrintFs(uint32_t inode, uint32_t j){
    i_node_t* inFile = get_inode(inode);
    dir_t* dir = (dir_t*)inFile->pages[0];
    uint32_t i,k;
    i_node_t* child; 

    for(i = 2; i < dir->num_childs; i++){
        for(k = 0; k < 3*j;k++)
            uart_putc(' ');
        child = get_inode(dir->child[i].inode_num);
        uart_putln(dir->child[i].filename);
        if(child->type == 1)
            recPrintFs(dir->child[i].inode_num, j + 1);
        
    }
    return;
}

void printFs(void){
    recPrintFs(0,0);
}