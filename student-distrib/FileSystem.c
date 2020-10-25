/* FileSystem.c - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */

#include "FileSystem.h"
#include "lib.h"

/*
 NAME: read_dentry_by_name
 DESCRIPTION: Scans through the directory entries in the boot block to find
              the file name, and then update the dentry passed to it
 IO: file name, and temp dentry
 IMPACTS ON OTHERS: updates dentry struct
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    uint32_t i;
    for(i = 0; i < boot_block_main.dir_count; i++){
        // compare input filename, with all filenames in the direcotry
        if (strncmp((int8_t*)boot_block_main.direntries[i].filename, (int8_t*)fname, FILENAME_LEN) == 0){
            return read_dentry_by_index(i, dentry); 
        }
    }

    // if fail, return -1
    return -1;
}

/*
 NAME: read_dentry_by_index
 DESCRIPTION: Scans through the directory entries in the boot block to find
              the file at the passed index, and then update the dentry passed to it
 IO: file index, and temp dentry
 IMPACTS ON OTHERS: Writes to dentry array
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if (index < boot_block_main.dir_count){
        // checks that index passed in is valid
        // copies filename to temp dentry
        strncpy(dentry->filename, boot_block_main.direntries[index].filename, FILENAME_LEN);
        // copies the fieltypes and filenames
        dentry->filetype = boot_block_main.direntries[index].filetype;
        dentry->inode_num = boot_block_main.direntries[index].inode_num;
        return 0;
    }
    
    // if fail, return -1
    return -1;
}

/*
 NAME: read_data
 DESCRIPTION: Reads data of inode block specified with some offset, store length
              number of bytes into buf
 IO: inode struct, offset amnt, buf pointer, and output length
 IMPACTS ON OTHERS: Writes to buf
 */
int32_t read_data (inode_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    const uint32_t* temp_ptr = boot_block_ptr;  // grabs the pointer to first block
    uint8_t* char_ptr;                          // used to temporarily loop thorugh chars
    int i, loop, flag, counter;
    flag = 0;
    i = 0;
    for (loop = 0; loop < (inode.length/(blocksizenorm*4)+1); loop++){
        // skips past boot_blk, past all inodes, and then points to correct data_blk
        temp_ptr = boot_block_ptr + blocksizenorm * (1 + boot_block_main.inode_count + inode.data_block_num[loop]);
        
        if ((loop+1) * 4096 < length){
            i = 4096;
        }              // sets the amount of iterations for the data blks
        
        else {
            i = 4096-(((loop+1) * 4096)-length);
        }
        
        char_ptr = (uint8_t*)temp_ptr;
        // check if elf or not
        if(*temp_ptr == 0x464C457F) flag = 1; 

        if (flag){
            counter = 0;
            while (i > 0) {
                if (*char_ptr == 0x00 || *char_ptr == 0x0A){
                    char_ptr++;
                    i--;
                }
                else {
                    putc(*char_ptr);
                    char_ptr++;
                    i--;
                    counter++;
                    if (counter == 80){
                        printf("\n");
                        counter = 0;
                    } 
                }
            }
        }

        else {
            while (i > 0) {
                putc(*char_ptr);
                char_ptr++;
                i--;
            }
        }
    }

    return 0;
}

/*
 NAME: file_open
 DESCRIPTION: Assigns an inode to a file and opens it. Places the inode number and offset
              into a primitive pcb list.
 IO: filename to open
 IMPACTS ON OTHERS: Will affect inode_list, unlocks file to be read
 */
int file_open(const uint8_t* fname){
    dentry_t temp;
    int i;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp) == 0 && open_file_count < 8){
        for (i = 0; i < inodeamnt; i++){
        // looks through exisiting list
            if (temp.inode_num != inode_list[i].inode_num || inode_list[i].inode_num == 0 || inode_list[i].inode_num == -1){  
            // checks if inode already exists, and that it is empty
                inode_list[i].inode_num = temp.inode_num;
                inode_list[i].offset = 0;        
                open_file_count++; // used to keep count of open files
                return 0;
            }
        }
    }

    // if file does not exist, or if too many files are open
    return -1;
}

/*
 NAME: file_open
 DESCRIPTION: Removes the inode number and offset from the primitive pcb list.
 IO: filename to close
 IMPACTS ON OTHERS: Will affect inode_list, locks file to be read
 */
int file_close(const uint8_t* fname){
    dentry_t temp;
    int i;
    // looks for file, grabs inode number, attempts to close
    if (read_dentry_by_name(fname, &temp) == 0 && open_file_count > 0){
        for (i = 0; i < inodeamnt; i++){
        // looks through exisitng list
            if (temp.inode_num == inode_list[i].inode_num){
            // checks if inode exists, as open
                inode_list[i].inode_num = -1;
                inode_list[i].offset = 0;        
                open_file_count--;  // used to keep track of open files
                return 0;
            }
        }
    }

    // if file does not exist, or if none are open
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
    found = -1;         // set to fail condition, if unchanged is fail
    const uint32_t* temp_ptr = boot_block_ptr;

    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(fname, &temp_dentry) == 0 && open_file_count > 0){
        for (i = 0; i < inodeamnt; i++){
            // checks to make sure the file to be read is already open
            if (temp_dentry.inode_num == inode_list[i].inode_num){  
                found = i;
            }
        }
        if (found == -1) return -1; // file not currently open, fail
    }

    // find inode here
    temp_ptr = temp_ptr + blocksizenorm * (1+temp_dentry.inode_num);

    temp_inode.length = *temp_ptr;
    i = temp_inode.length/(blocksizenorm*4) + 1;

    x = 0;
    while (i>0) {
        // update temp inode struct with correct amnt of data blks
        temp_inode.data_block_num[x] = *(temp_ptr + x + 1);
        x++;    //increment struct array ptr to fill next data blk number
        i--;    // decrement counter for data blocks left
    }

    // pass this inode data to this fucntion to fill bufffer
    return read_data(temp_inode, inode_list[found].offset, buf, temp_inode.length);
}

/*
 NAME: dir_open
 DESCRIPTION: opens a directory file
 IO: NONE
 IMPACTS ON OTHERS: initializes the boot_block struct
 */
int dir_open(void){
    int i, floop;
    int8_t* chars;
    const uint32_t* temp_ptr = boot_block_ptr;
    open_file_count = 0;                        // initialize the amnt of files open

    boot_block_main.dir_count = *temp_ptr;      // set ptr to beginnning of file sys, dentries count
    boot_block_main.inode_count = *(temp_ptr+1);// grab the next 4 bytes, inode blk count
    boot_block_main.data_count = *(temp_ptr+2); // grab the enxt 4 bytes, data blk count

    temp_ptr = temp_ptr + inodeamnt*2;          // move ptr to beginning of dentries

    // fills all dentries
    for(i = 0; i < boot_block_main.dir_count; i++){
        chars = (int8_t*)(temp_ptr);            // useful for grabbing char by char
        for (floop = 0; floop < FILENAME_LEN; floop++){
            boot_block_main.direntries[i].filename[floop] = *(chars+floop); // grabs chars
        }
        boot_block_main.direntries[i].filetype = *(temp_ptr+inodeamnt);     // moves pointer to point to filetype
        boot_block_main.direntries[i].inode_num = *(temp_ptr+inodeamnt+1);  // moves pointer to point to inode num
        temp_ptr = temp_ptr + inodeamnt*2;                                  // moves pointer to next dentry to copy
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
    // unused function
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
    // loops through all dentries in boot blk
    for(i = 0; i < boot_block_main.dir_count; i++){
        read_dentry_by_index(i, &temp);
        printf("file_name: ");                              // prints filename 
        for (floop = 0; floop < FILENAME_LEN; floop++){
			printf("%c", temp.filename[floop]);
        }
        printf(", file_type: ");                            // print file type
		printf("%d", temp.filetype);
		printf(", inode_num: %d\n"   , temp.inode_num);     // prints inode number for file
    }
    return 0;
}
