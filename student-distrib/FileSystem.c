/* FileSystem.c - This sets up file system structs, and fle system funcs.
 * vim:ts=4 noexpandtab
 */

#include "FileSystem.h"
#include "lib.h"
#include "sys_calls.h"

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
        return index;
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

 // does not match spec regarding first parameter
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    const uint32_t* temp_ptr = boot_block_ptr;  // grabs the pointer to first block
    uint8_t* char_ptr;                          // used to temporarily loop thorugh chars
    int i, loop, min_block, max_block, length_read;
    i = 0;
    loop = 0;
    length_read = 0;
    int32_t * temp_inode = (int32_t *)(boot_block_ptr+blocksizenorm*(1+inode));

    min_block = offset/(COMMON_SIZE*4);
    // we add plus one to get the remaining bytes in the file
    max_block = (*temp_inode/(COMMON_SIZE*4)) + 1;

    for (loop = min_block; loop < max_block; loop++){
        // skips past boot_blk, past all inodes, and then points to correct data_blk
        temp_ptr = boot_block_ptr + blocksizenorm * (1 + boot_block_main.inode_count + *(temp_inode + 1 + loop));
        char_ptr = (uint8_t*)temp_ptr;

        // length_read does not exceed length amnt
        // bytes_read does not exceed inode_length
        // skip to next data block every (4096 bytes_read)
        if(*temp_inode > ((COMMON_SIZE*4))*(loop+1)){
            i = (((COMMON_SIZE*4))*(loop+1)) - offset;
        }
        else{
            i = *temp_inode - offset;
        }

        if(i > length - length_read){
            i = length - length_read;
        }

        // find current position in data block with bytes_read%4096;
        char_ptr = char_ptr + (offset%(COMMON_SIZE*4));
        while (i > 0) {
            // place all chars into buffer
            *(buf+length_read) = *char_ptr;
            char_ptr++;

            offset++;
            length_read++;
            i--;
        }
        // return if we hit the length bytes read
        if (length_read == length) return length_read;
    }
    return length_read;
}

/*
 NAME: file_open
 DESCRIPTION: Assigns an inode to a file and opens it. Places the inode number and offset
              into a primitive pcb list.
 IO: filename to open
 IMPACTS ON OTHERS: Will affect inode_list, unlocks file to be read
 */
int32_t file_open(const uint8_t* filename){
    // check if filename exists, then handle opening in sys_calls
    return 0; // read_dentry_by_name(filename, &temp);
}

/*
 NAME: file_close
 DESCRIPTION: Removes the inode number and offset from the primitive pcb list.
 IO: filename to close
 IMPACTS ON OTHERS: Will affect inode_list, locks file to be read
 */
int32_t file_close(int32_t fd){
    // handle this in sys_calls with access to fd table
    return 0;
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
    // confirm that nbytes is valid
    if (nbytes < 0)
      return -1;

    // confirm that we haven't read the whole file
    const uint32_t * placeholder = boot_block_ptr + blocksizenorm*(1+all_pcbs[pid_counter].fdt[fd].inode);
    if(all_pcbs[pid_counter].fdt[fd].file_bytes_read == *placeholder){
        // all_pcbs[pid_counter].fdt[fd].file_bytes_read = 0;
        return 0;
    }

    // read the data
    return read_data(all_pcbs[pid_counter].fdt[fd].inode,
                      all_pcbs[pid_counter].fdt[fd].file_bytes_read, buf, nbytes);
}

/*
 NAME: dir_open
 DESCRIPTION: opens a directory file
 IO: NONE
 IMPACTS ON OTHERS: initializes the boot_block struct
 */
int32_t dir_open(const uint8_t* filename){
    if (*filename != '.') return -1;
    if (dir_is_open) {
        curr_dir = 0;
        return 0;
    }

    int i, floop;
    int8_t* chars;
    const uint32_t* temp_ptr = boot_block_ptr;
    curr_dir = 0;                                   // initializes directory index
    boot_block_main.dir_count = *temp_ptr;          // set ptr to beginnning of file sys, dentries count
    boot_block_main.inode_count = *(temp_ptr+1);    // grab the next 4 bytes, inode blk count
    boot_block_main.data_count = *(temp_ptr+2);     // grab the enxt 4 bytes, data blk count

    temp_ptr = temp_ptr + inodeamnt*2;              // move ptr to beginning of dentries

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
    dir_is_open = 1;
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
    if (dir_is_open == 0) return -1;
    int floop;
    dentry_t temp;
    uint8_t* temp_ptr = buf;
    // loops through all dentries in boot blk
    if(curr_dir < boot_block_main.dir_count){
        read_dentry_by_index(curr_dir, &temp);
        //printf("file_name: ");                              // prints filename
        for (floop = 0; floop < FILENAME_LEN; floop++){
			//printf("%c", temp.filename[floop]);
            if (temp.filename[floop] == '\0') break;
            *(temp_ptr+floop) = temp.filename[floop];
        }
        //printf(", file_type: ");                            // print file type
		//printf("%d", temp.filetype);
		//printf(", inode_num: %d\n"   , temp.inode_num);     // prints inode number for file
        curr_dir++;
        return floop;
    }

    curr_dir = 0;
    return 0;
}

/*
 NAME: exec_check
 DESCRIPTION: Checks if first 4 bytes are ELF
 IO: NONE
 IMPACTS ON OTHERS: None
 */
int32_t exec_check(uint32_t inode){

    const uint32_t* temp_ptr;

    // get the inode block's address
    temp_ptr = boot_block_ptr + blocksizenorm * (1+inode);
    // get the first data block's address
    temp_ptr = boot_block_ptr + blocksizenorm * (1 + boot_block_main.inode_count + *(temp_ptr + 1));

    // check if elf or not
    if(*temp_ptr != ELFMAGIC) return -1;

    return (*(temp_ptr+6));
}

int32_t exec_file_load(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    // confirm valid read length
    if (length < 0)
      return -1;

    // confirm that we haven't finished reading the file
    // this is probably unnecessary given our implementation
    const uint32_t * placeholder = boot_block_ptr + blocksizenorm*(1+inode);
    if(offset == *placeholder)
        return 0;

    // read the data
    return read_data(inode, offset, buf, length);

}
