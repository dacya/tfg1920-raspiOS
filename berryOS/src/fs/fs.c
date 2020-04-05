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
#define NUM_INODES ((PAGE_SIZE/sizeof(i_node_t))*NUM_PAGES_INODE_TABLE) //340
#define NUM_INODES_PER_PAGE (NUM_INODES/NUM_PAGES_INODE_TABLE)          //170

i_node_t* i_node_list_pages[NUM_PAGES_INODE_TABLE];

static i_node_t* get_inode(uint32_t num){
    return (i_node_t*)(((int)i_node_list_pages[num / NUM_INODES_PER_PAGE]) + (num % NUM_INODES_PER_PAGE)*sizeof(i_node_t)); 
}

dir_t* root_dir;
dir_t* curr_dir;
int freeInodes;
uint32_t minFreeInode;

static uint32_t getFreeInode(){
    i_node_t* pointer;
    uint32_t i;
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

static uint32_t fileExists(char* file){  //0 if file doesn't exist in the current directory, number of child if exists
    uint32_t i = 2;
    while(i < curr_dir->num_childs){
        if (streq(file,curr_dir->child[i].filename))
            return i;
        i++;
    }
    return 0;
}

void fs_init(void){
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
    
    memcpy(root_dir->real_name, "root\0", 5);
    root_dir->real_name_size = 5;    
    root_dir->num_childs = 2;

    memcpy(root_dir->child[0].filename, ".\0", 2);
    root_dir->child[0].fn_size = 2;
    root_dir->child[0].inode_num = 0;
    
    memcpy(root_dir->child[1].filename, "..\0", 3);
    root_dir->child[1].fn_size = 3;
    root_dir->child[1].inode_num = 0;
    
    curr_dir = root_dir; //Root is the current directory

    freeInodes = NUM_INODES - 1; //Num of free inodes
    minFreeInode = 1;
}

void createFile(char* file, int fnsize){
    if (freeInodes == 0 || curr_dir->num_childs == MAXFILESPERDIR || fileExists(file) != 0)
        return;
    void* aux = alloc_page();
    
    if(aux == NULL)
        return;

    uint32_t new = getFreeInode();
    i_node_t* newPoint = get_inode(new);
    
    newPoint->pages[0] = aux;
    newPoint->free = 1;
    newPoint->num_pages = 1;
    newPoint->size = 0;
    newPoint->type = 0;
    
    memcpy(curr_dir->child[curr_dir->num_childs].filename, file, MIN(MAXFILENAMESIZE, fnsize));
    curr_dir->child[curr_dir->num_childs].filename[MIN(MAXFILENAMESIZE - 1, fnsize)] = '\0';
    curr_dir->child[curr_dir->num_childs].fn_size = MIN(MAXFILENAMESIZE - 1, fnsize);
    curr_dir->child[curr_dir->num_childs].inode_num = new;
    curr_dir->num_childs++;
    freeInodes--;
}




int write(char* filename, char* text){
    int i;
    if ((i = fileExists(filename)) == 0)
        return 0;
    
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    char* file = ((char*)inFile->pages[inFile->size/PAGE_SIZE]) + (inFile->size % PAGE_SIZE);
    
    i = 0;
    while(*text != '\0'){
        if((i + inFile->size) % PAGE_SIZE == 0 && (i + inFile->size) != 0){
            uart_putln("No me mires");
            void* aux;
            if(inFile->num_pages == MAXPAGESPERFILE || (aux = alloc_page()) == NULL){
                inFile->size += i;
                return i;
            }
            inFile->pages[inFile->num_pages++] = aux;
            file = aux;    
        }
        *file = *text;
        i++;
        text++;
        file++;
        
    }
    inFile->size += i;
    return i;
}

char* read(char* filename, uint32_t bytes){
    
    uint32_t i;
    if ((i = fileExists(filename)) == 0)
        return NULL;
    
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    bytes = MIN(bytes, inFile->size);
    char* sol;
    if ((sol = kmalloc(bytes + 1)) == NULL)
        return NULL;

    int j = 0;
    char* file = (char *)inFile->pages[j++];
    char* aux = sol;

    i = 0;
    while(i < bytes){
        if(i % PAGE_SIZE == 0 && i != 0){
            uart_putln("No me mires");
            file = (char *)inFile->pages[j++];
        }
        *aux = *file;
        i++;
        aux++;
        file++;
    }
    sol[bytes] = '\0';
    return sol;

}

int getFileSize(char* filename){
    int i;
    if ((i = fileExists(filename)) == 0)
        return 0;
    
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    
    return inFile->size;
}

void deleteFile(char* filename){
    uint32_t i;
    if ((i = fileExists(filename)) == 0)
        return;
    
    i_node_t* inFile = get_inode(curr_dir->child[i].inode_num);
    
    if(curr_dir->child[i].inode_num < minFreeInode)
        minFreeInode = curr_dir->child[i].inode_num;
    
    bzero2(curr_dir->child[i].filename, curr_dir->child[i].fn_size);
    curr_dir->child[i].fn_size = 0;
    curr_dir->child[i].inode_num = 0;
    curr_dir->num_childs--; 
    
    for(i = 0; i < inFile->num_pages; i++){
        free_page(inFile->pages[i]);
        uart_hex_puts((int)inFile->pages[i]);
        inFile->pages[i] = 0;
    }

    inFile->free = 0;
    inFile->size = 0;
    inFile->type = 0;
}

void printCurrDir(){
    uint32_t i = 0;
    
    while(i < curr_dir->num_childs){
        uart_puts(curr_dir->child[i].filename);
        uart_puts("--> in = ");
        uart_putln(itoa(curr_dir->child[i].inode_num));
        i++;
    }
}

