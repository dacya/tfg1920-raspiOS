#include <fs/fs.h>
#include <mem/mem.h>
#include <utils/stdlib.h>

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
int minInodeFree;

static uint32_t getFreeInode(){
    i_node_t* pointer;
    for(int i = minInodeFree + 1; i < NUM_INODES; i++){
        pointer = get_inode(1);
        if(pointer->free == 0){
            int aux = minInodeFree;
            minInodeFree = i;
            return aux;
        }
    }
    return 0;
}

void fs_init(void){
    int i;
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
    root_dir = iter->pages[0];
    
    memcpy(root_dir->real_name, "root\0", 5);
    root_dir->real_name_size = 5;    
    root_dir->num_childs = 2;

    memcpy(root_dir->child[0].filename, ".\0", 2);
    root_dir->child[0].fn_size = 2;
    root_dir->child[0].inode_num = 0;
    
    memcpy(root_dir->child[1].filename, "..\0", 3);
    root_dir->child[1].fn_size = 3;
    root_dir->child[1].inode_num = 0;
    iter->pages[0] = root_dir;
    
    
    curr_dir = root_dir; //Root is the current directory

    freeInodes = NUM_INODES - 1; //Num of free inodes
    minInodeFree = 1;
}

void createFile(char* file, int fnsize){
    if (freeInodes == 0 || curr_dir->num_childs == MAXFILESPERDIR)
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
    
    memcpy(curr_dir->child[0].filename, file, MIN(MAXFILENAMESIZE, fnsize));
    curr_dir->child[curr_dir->num_childs++].filename[MIN(MAXFILENAMESIZE - 1, fnsize)] = '\0';
    curr_dir->child[curr_dir->num_childs - 1].fn_size = MIN(MAXFILENAMESIZE - 1, fnsize);
    curr_dir->child[curr_dir->num_childs - 1].inode_num = new;
}

