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
    // Scans through the directory entries in the “boot block” to find the file name >
    // Call read_dentry_by_index() {
        // Populates the dentry parameter -> file name, file type, inode number >
    uint32_t i;
    for(i = 0; i < boot_block_main.dir_count; i++){
        if (strncmp((int8_t*)boot_block_main.direntries[i].filename, (int8_t*)fname, FILENAME_LEN) == 0){
            return read_dentry_by_index(i, dentry); 
        }
    }
    return -1;
}

/*
 NAME: read_dentry_by_index
 DESCRIPTION: Populates the dentry parameter -> file name, file type, inode num.
              Create a temporary dentry and populates it before adding it to dentry
 IO: NONE
 IMPACTS ON OTHERS: Writes to dentry array
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if (index < boot_block_main.dir_count){
        strncpy(dentry->filename, boot_block_main.direntries[index].filename, FILENAME_LEN);
        dentry->filetype = boot_block_main.direntries[index].filetype;
        dentry->inode_num = boot_block_main.direntries[index].inode_num;
        return 0;
    }
    
    return -1;
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
    int floop;
    dentry_t temp;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp) == 0){
        printf("file_name: ");
        for (floop = 0; floop < FILENAME_LEN; floop++){
			printf("%c", temp.filename[floop]);
        }
        printf(", file_type: ");
		printf("%d", temp.filetype);
		printf(", inode_num: %d\n"   , temp.inode_num);
    }

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
int file_write(void){
    // unused function
    return -1;
}

/*
 NAME: file_read
 DESCRIPTION: Reads file given by fname and stores it into buf
 IO: NONE
 IMPACTS ON OTHERS: Will return an error if you try to read this file again
 */
int file_read(const uint8_t* fname, uint8_t* buf){
    /*
    int i, loop1;
    
    // looks for file, grabs inode number, marks as opened
    for(i = 0; i < boot_block_main.dir_count; i++){
        if (strncmp(boot_block_main.direntries[i].filename, fname, strlen(fname))){
            loop1 = 0;
            while(inode_list[loop1].inode_num != boot_block_main.direntries[i].inode_num && loop1 < 8){
                loop1++;
                if(loop1 == 8) return -1;
            }
            


            read_data(inode_list[loop1].inode_num, inode_list[loop1].offset, &buf, )
        }
        else return -1;
    }
    */
    return 0;
}

/*
 NAME: dir_open
 DESCRIPTION: opens a directory file
 IO: NONE
 IMPACTS ON OTHERS:
 */
int dir_open(void){
    int i, floop;
    int8_t* chars;
    const uint32_t* temp_ptr = boot_block_ptr;

    boot_block_main.dir_count = *temp_ptr;
    boot_block_main.inode_count = *(temp_ptr+1);
    boot_block_main.data_count = *(temp_ptr+2);

    temp_ptr = temp_ptr + 16;

    // fills all dentries
    for(i = 0; i < boot_block_main.dir_count; i++){
        chars = (int8_t*)(temp_ptr);
        for (floop = 0; floop < FILENAME_LEN; floop++){
            boot_block_main.direntries[i].filename[floop] = *(chars+floop);
        }
        boot_block_main.direntries[i].filetype = *(temp_ptr+8);
        boot_block_main.direntries[i].inode_num = *(temp_ptr+9);
        temp_ptr = temp_ptr + 16;
    }

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
    return -1;
}

/*
 NAME: dir_read
 DESCRIPTION: Reads all files like ls
 IO: NONE
 IMPACTS ON OTHERS: None
 */
int dir_read(void){
    int i, floop;
    dentry_t temp;
	clear();
    for(i = 0; i < boot_block_main.dir_count; i++){
        read_dentry_by_index(i, &temp);
        printf("file_name: ");
        for (floop = 0; floop < FILENAME_LEN; floop++){
			printf("%c", temp.filename[floop]);
        }
        printf(", file_type: ");
		printf("%d", temp.filetype);
		printf(", inode_num: %d\n"   , temp.inode_num);
    }
    return 0;
}
