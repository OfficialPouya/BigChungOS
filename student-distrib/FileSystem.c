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
int32_t read_data (inode_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    // go into data block specified by inode[offset], write length number of
    // bytes into buf. buf will be printed to the terminal elsewhere
    const uint32_t* temp_ptr = boot_block_ptr;
    uint8_t* char_ptr;
    int i, loop;

    for (loop = 0; loop < (inode.length/4096+1); loop++){
        temp_ptr = temp_ptr + 1024 * (1 + boot_block_main.inode_count + inode.data_block_num[loop]);
        i = 1024;
        char_ptr = (uint8_t*)temp_ptr;
        while (i > 0) {
            putc(*char_ptr);
            char_ptr++;
            i--;
        }
    }

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
    dentry_t temp;
    int i;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp) == 0 && open_file_count < 8){
        for (i = 0; i < 8; i++){
            if (temp.inode_num != inode_list[i].inode_num && inode_list[i].inode_num == 0){  
                inode_list[i].inode_num = temp.inode_num;
                inode_list[i].offset = 0;        
                open_file_count++;
                return 0;
            }
        }
    }

    return -1;
}

/*
 NAME: file_close
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int file_close(const uint8_t* fname){
    // maybe free the inode from the file? idk if this is necessary
    dentry_t temp;
    int i;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp) == 0 && open_file_count > 0){
        for (i = 0; i < 8; i++){
            if (temp.inode_num == inode_list[i].inode_num){  
                inode_list[i].inode_num = -1;
                inode_list[i].offset = 0;        
                open_file_count--;
                return 0;
            }
        }
    }

    return -1;
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
    dentry_t temp_dentry;
    inode_t temp_inode;
    int i, x, found;
    found = -1;
    const uint32_t* temp_ptr = boot_block_ptr;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp_dentry) == 0 && open_file_count > 0){
        for (i = 0; i < 8; i++){
            if (temp_dentry.inode_num == inode_list[i].inode_num){  
                found = i;
            }
        }
        if (found == -1) return -1;
    }

    // find inode here
    temp_ptr = temp_ptr + 1024*(1+temp_dentry.inode_num);

    temp_inode.length = *temp_ptr;
    i = temp_inode.length/4096 + 1;

    x = 0;
    while (i>0) {
        temp_inode.data_block_num[x] = *(temp_ptr + x + 1);
        x++;
        i--;
    }

    return read_data(temp_inode, inode_list[found].offset, buf, 1024);
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
    open_file_count = 0;

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
