/* FileSystem.h - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"
#include "x86_desc.h"

#define FILENAME_LEN    32
#define reserved24B     24
#define reserved52B     52
#define maxdirentries   63
#define maxblocksize    1023
#define blocksizenorm   1024
#define inodeamnt       8
#define ELFMAGIC        0x464C457F

// NEED TO REWRITE THIS FILE

typedef struct dentry_t {
    int8_t      filename[FILENAME_LEN];
    int32_t     filetype;
    int32_t     inode_num;
    int8_t      reserved[reserved24B];
} dentry_t;

typedef struct boot_block_t {
    int32_t     dir_count;
    int32_t     inode_count;
    int32_t     data_count;
    int8_t      reserved[reserved52B];
    dentry_t    direntries[maxdirentries];
} boot_block_t;

typedef struct inode_t {
    int32_t     length;
    int32_t     data_block_num[maxblocksize];
} inode_t;

int curr_dir;
int bytes_read;
int dir_is_open;

const uint32_t* boot_block_ptr;
boot_block_t boot_block_main;

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (inode_t* inode, uint32_t offset, uint8_t* buf, uint32_t length);

// FILE FUNCTIONS
// initialize any temp structs, return 0 success
int32_t file_open(const uint8_t* filename);

// undo file_open function
int32_t file_close(int32_t fd);

// should do nothing, return -1
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);

// reads count bytesof data from file into buf
int32_t file_read(const uint8_t* filename, void* buf, int32_t nbytes);

// DIRECTORY FUNCTIONS
// opens a directory file, return 0 with success
int32_t dir_open(const uint8_t* filename);

// probably does nothing, return 0
int32_t dir_close(int32_t fd);

// should do nothing, return 0
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);

// reads filename by filename, return 0 on success
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);

// Checks if first 4 bytes are ELF
int32_t exec_check(const uint8_t* filename);

#endif
