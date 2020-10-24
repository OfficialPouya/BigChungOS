/* FileSystem.h - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */

#include "types.h"
#include "x86_desc.h"

#define FILENAME_LEN    32  
#define reserved24B     24 
#define reserved52B     52 
#define maxdirentries   63
#define maxblocksize    1023
#define blocksizenorm   1024
#define inodeamnt       8

typedef struct dentry {
    int8_t      filename[FILENAME_LEN];
    int32_t     filetype;
    int32_t     inode_num;
    int8_t      reserved[reserved24B];
} dentry_t;

typedef struct boot_block {
    int32_t     dir_count;
    int32_t     inode_count;
    int32_t     data_count;
    int8_t      reserved[reserved52B];
    dentry_t    direntries[maxdirentries];
} boot_block_t;

typedef struct inode {
    int32_t     length;
    int32_t     data_block_num[maxblocksize];
} inode_t;

typedef struct curr_inode {
    int32_t     inode_num;
    int32_t     offset;
} curr_inode_t;

int open_file_count;
curr_inode_t inode_list[inodeamnt];
const uint32_t* boot_block_ptr;
boot_block_t boot_block_main;

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (inode_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

// FILE FUNCTIONS
// initialize any temp structs, return 0 success
int file_open(const uint8_t* fname);

// undo file_open function
int file_close(const uint8_t* fname);

// should do nothing, return -1
int file_write(void);

// reads count bytesof data from file into buf
int file_read(const uint8_t* fname, uint8_t* buf);

// DIRECTORY FUNCTIONS
// opens a directory file, return 0 with success
int dir_open(void);

// probably does nothing, return 0
int dir_close(void);

// should do nothing, return 0
int dir_write(void);

// reads filename by filename, return 0 on success
int dir_read(void);
