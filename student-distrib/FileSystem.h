/* FileSystem.h - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */

#include "types.h"
#include "x86_desc.h"

#define example       0x1    

typedef struct dentry {
    char fname[32];
    uint32_t ftype;
    uint32_t inode;
    uint8_t padding[24];
} dentry_t;

uint32_t curr_inodes[2][8];

uint32_t boot_block_ptr;

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

int file_open(const uint8_t* fname);
int file_close(const uint8_t* fname);
int file_write();
int file_read(const uint8_t* fname);

int dir_open(void);
int dir_close(void);
int dir_write(void);
int dir_read(void);