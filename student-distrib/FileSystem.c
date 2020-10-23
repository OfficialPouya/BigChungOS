/* FileSystem.c - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */

#include "FileSystem.h"
#include "lib.h"

/*
 NAME: read_dentry_by_name
 DESCRIPTION: Scans through the directory entries in the boot block to find
              the file name
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    // this finds the dentry for a file, used in/for other functions?
    return 0;
}

/*
 NAME: read_dentry_by_index
 DESCRIPTION: Populates the dentry parameter -> file name, file type, inode num.
              Create a temporary dentry and populates it before adding it to dentry
 IO: NONE
 IMPACTS ON OTHERS: Writes to dentry array
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    return 0;
}

/*
 NAME: read_data
 DESCRIPTION: Reads data of inode block specified with some offset, store length
              number of bytes into buf
 IO: NONE
 IMPACTS ON OTHERS: Writes to buf
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    // go into data block specified by inode[offset], write length number of
    // bytes into buf. buf will be printed to the terminal elsewhere

    return 0;
}

/*
 NAME: file_open
 DESCRIPTION: Assigns an inode to a file and opens it
 IO: NONE
 IMPACTS ON OTHERS: Will affect curr_inodes
 */
int file_open(const uint8_t* fname){
    // assign a free inode to fname and puts it into file descriptor (not this
    // checkpoint). this opens the file and allows it to be read
    return 0;
}

/*
 NAME: file_close
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int file_close(const uint8_t* fname){
    // maybe free the inode from the file? idk if this is necessary
    return 0;
}

/*
 NAME: file_write
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int file_write(){
    // unused function
    return 0;
}

/*
 NAME: file_read
 DESCRIPTION: Reads file given by fname and stores it into buf
 IO: NONE
 IMPACTS ON OTHERS: Will return an error if you try to read this file again
 */
int file_read(const uint8_t* fname, uint8_t* buf){
    // i think this is used in conjunction with other functions, maybe
    // read_dentry_by_name->read_data?
    return 0;
}

/*
 NAME: dir_open
 DESCRIPTION: opens a directory file
 IO: NONE
 IMPACTS ON OTHERS:
 */
int dir_open(void){
    // need more details
    return 0;
}

/*
 NAME: dir_close
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int dir_close(void){

    return 0;
}

/*
 NAME: dir_write
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int dir_write(void){
    // not used
    return 0;
}

/*
 NAME: dir_read
 DESCRIPTION: Reads all files like ls
 IO: NONE
 IMPACTS ON OTHERS: None
 */
int dir_read(void){
    // this is used to print out every file in directory to screen like in gif
    // i think
    return 0;
}
