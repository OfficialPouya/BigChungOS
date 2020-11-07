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
    flag = 0;       // check if exe or not
    i = 0;
    bytes_read = 0; // sets this back to 0 before reading more

    for (loop = 0; loop < (length/(blocksizenorm*4)+1); loop++){
        // skips past boot_blk, past all inodes, and then points to correct data_blk
        temp_ptr = boot_block_ptr + blocksizenorm * (1 + boot_block_main.inode_count + inode.data_block_num[loop]);
        
        // sets the amount of iterations for the data blks
        // lets loop write full 4096 sized block
        if ((loop+1) * (blocksizenorm*4) < length){
            i = (blocksizenorm*4);
        }              
        
        // only allows left over bytes to be read
        else {
            i = (blocksizenorm*4)-(((loop+1) * (blocksizenorm*4))-length);
        }
        
        char_ptr = (uint8_t*)temp_ptr;
        // check if elf or not
        // if(*temp_ptr == ELFMAGIC) flag = 1; 

        // // if executable, do this
        // if (flag){
        //     counter = 0;
        //     while (i > 0) {
        //         // skip over null and endline chars
        //         if (*char_ptr == 0x00 || *char_ptr == nextline){
        //             char_ptr++;
        //             i--;
        //         }
        //         // put other chars into buffer
        //         else {
        //             *(buf+bytes_read) = *char_ptr;
        //             char_ptr++;
        //             bytes_read++;
        //             i--;
        //             counter++;
        //             // need to add new line, since we are avoiding all given ones
        //             if (counter == screensize){
        //                 //*(buf+bytes_read) = nextline;
        //                 //bytes_read++;
        //                 counter = 0;
        //             } 
        //         }
        //     }
        // }

        // if not executable, do this
        //else {
            while (i > 0) {
                // place all chars into buffer
                *(buf+bytes_read) = *char_ptr;
                char_ptr++;
                bytes_read++;
                i--;
            }
        //}
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
int32_t file_open(const uint8_t* filename){
    dentry_t temp;
    int i, floop;
    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name(filename, &temp) == 0 && open_file_count < 8){
        for (i = 3; i < inodeamnt; i++){
        // looks through exisiting list
            if (temp.inode_num != inode_list[i].inode_num && (inode_list[i].inode_num == 0 || inode_list[i].inode_num == -1)){  
            // checks if inode already exists, and that it is empty
                inode_list[i].inode_num = temp.inode_num;
                inode_list[i].offset = 0; 
                inode_list[i].filetype = temp.filetype;
                for (floop = 0; floop < FILENAME_LEN; floop++){
                    inode_list[i].filename[floop] = temp.filename[floop];  
                }
                open_file_count++; // used to keep count of open files
                return i;
            }
        }
    }

    // if file does not exist, or if too many files are open
    return -1;
}

/*
 NAME: file_close
 DESCRIPTION: Removes the inode number and offset from the primitive pcb list.
 IO: filename to close
 IMPACTS ON OTHERS: Will affect inode_list, locks file to be read
 */
int32_t file_close(int32_t fd){
    dentry_t temp;
    int i, floop;
    // looks for file, grabs inode number, attempts to close
    if (read_dentry_by_name((uint8_t*)inode_list[fd].filename, &temp) == 0 && open_file_count > 3){
        for (i = 3; i < inodeamnt; i++){
        // looks through exisitng list
            if (strncmp((int8_t*)temp.filename, (int8_t*)inode_list[i].filename, FILENAME_LEN) == 0){
            // checks if inode exists, as open
                inode_list[i].inode_num = -1;
                inode_list[i].offset = 0; 
                inode_list[i].filetype = -1;
                for (floop = 0; floop < FILENAME_LEN; floop++){
                    inode_list[i].filename[floop] = (uint8_t)"";  
                }   
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
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes){
    // unused function
    return -1;
}

/*
 NAME: file_read
 DESCRIPTION: Reads file given by fname and stores it into buf
 IO: NONE
 IMPACTS ON OTHERS: Will return an error if you try to read this file again
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes){
    dentry_t temp_dentry;
    inode_t temp_inode;
    int i, x, found;
    found = -1;         // set to fail condition, if unchanged is fail
    const uint32_t* temp_ptr = boot_block_ptr;

    // looks for file, grabs inode number, marks as opened
    if (read_dentry_by_name((uint8_t*)inode_list[fd].filename, &temp_dentry) == 0 && open_file_count > 3){
        for (i = 3; i < inodeamnt; i++){
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

    if (nbytes<temp_inode.length) {
        i = nbytes/(blocksizenorm*4) + 1;
        x = 0;
        while (i>0) {
            // update temp inode struct with correct amnt of data blks
            temp_inode.data_block_num[x] = *(temp_ptr + x + 1);
            x++;    //increment struct array ptr to fill next data blk number
            i--;    // decrement counter for data blocks left
        }
        // pass this inode data to this fucntion to fill bufffer
        return read_data(temp_inode, inode_list[found].offset, buf, nbytes); 
    }

    else{
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
}

/*
 NAME: dir_open
 DESCRIPTION: opens a directory file
 IO: NONE
 IMPACTS ON OTHERS: initializes the boot_block struct
 */
int32_t dir_open(const uint8_t* filename){
    if (*filename != '.') return -1;
    
    int i, floop;
    int8_t* chars;
    const uint32_t* temp_ptr = boot_block_ptr;
    open_file_count = 3;                        // initialize the amnt of files open
    curr_dir = 0;                               // initializes directory index
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
int32_t dir_close(int32_t fd){
    // rests directory index
    curr_dir = 0;
    return 0;
}

/*
 NAME: dir_write
 DESCRIPTION: Does nothing
 IO: NONE
 IMPACTS ON OTHERS: NONE
 */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes){
    // not used
    return -1;
}

/*
 NAME: dir_read
 DESCRIPTION: Reads all files like ls
 IO: NONE
 IMPACTS ON OTHERS: None
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes){
    int floop;
    dentry_t temp;
    uint8_t* temp_ptr = buf;
    // loops through all dentries in boot blk
    if(curr_dir < boot_block_main.dir_count){
        read_dentry_by_index(curr_dir, &temp);
        printf("file_name: ");                              // prints filename 
        for (floop = 0; floop < FILENAME_LEN; floop++){
			printf("%c", temp.filename[floop]);
            *(temp_ptr+floop) = temp.filename[floop];
        }
        printf(", file_type: ");                            // print file type
		printf("%d", temp.filetype);
		printf(", inode_num: %d\n"   , temp.inode_num);     // prints inode number for file
        curr_dir++;
        return FILENAME_LEN;
    }

    return 0;
}
