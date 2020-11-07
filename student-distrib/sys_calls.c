
#include "sys_calls.h"

int fd_index_holder;

fop rtc_struct = {rtc_open, rtc_close, rtc_read, rtc_write};
fop file_struct = {file_open, file_close, file_read, file_write};
fop dir_struct = {dir_open, dir_close, dir_read, dir_write};
fop terminal_struct = {terminal_open, terminal_close, terminal_read, terminal_write};

void sys_call_handler() {
    printf("\nBad System Call\n");
}

int32_t sys_open(const uint8_t *filename) {
    
    // check params passed to sys call 
    if (filename[0] == 0) {return -1;}
    int f_o = file_open(filename);
    if(f_o == -1){return -1;}

    dentry_t temp_dentry;
    dentry_t *curr_dentry = &temp_dentry;
    // be able to see if file exists, if not return -1
    if(read_dentry_by_name(filename, curr_dentry) == -1){return -1;}
    // since exe & halt are index 0, 1 we start at 2, and there are 8 total calls
    int t_index;
    for(t_index=2; t_index<8; t_index++){
        // used : 1
        // empty : -1
        if(file_d_table[t_index].exists == -1){
            switch (file_d_table[t_index].file_type){
                case 0: 
                    file_d_table[t_index].fop_ = &rtc_struct;
                    break;
                case 1: 
                    file_d_table[t_index].fop_ = &dir_struct;
                    break;
                case 2: 
                    file_d_table[t_index].fop_ = &file_struct;
                    break;
            }
        file_d_table[t_index].exists = 1; // now it exists 
        fd_index_holder = t_index;
        return fd_index_holder;
        }
    }
    return -1; // nothing was able to get added 
}

int32_t sys_close(int32_t fd) {
    if (fd <= 2 || fd >= 6) {return -1;}
    if(file_d_table[fd].exists == 1){
        file_d_table[fd_index_holder].exists = -1;
        file_d_table[fd_index_holder].fop_->close(fd); 
    }
    return 0;
}

int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes) {
    if(!buf){return -1;}
    // the fd ranges from 0 - 6
    if (fd <= 0 || fd >= 6) {return -1;}
    if(file_d_table[fd_index_holder].exists == -1){return -1;}
    return file_d_table[fd_index_holder].fop_->write(fd, buf, nbytes); 
}


int32_t sys_read(int32_t fd, void *buf, int32_t nbytes) {
    if (!buf){return -1;}
    if (fd < 0 || fd >= 6 || fd == 1) {return -1;}
    if(file_d_table[fd_index_holder].exists == -1){return -1;}
    return file_d_table[fd_index_holder].fop_->read(fd, buf, nbytes);
}



int32_t sys_execute(const uint8_t *command){

return 0;
}

int32_t sys_halt(uint8_t status){

return 0;
}

void init_pcb(){
    int fdt_index;
    pcb my_pcb;
    for(fdt_index=0; fdt_index<8;fdt_index++){
        my_pcb.fdt[fdt_index].exists = -1;
    }
    // stdin members 
    file_d_table[0].fop_ = &terminal_struct;
    file_d_table[0].file_type = 3;
    // stdout members 
    file_d_table[1].fop_ = &terminal_struct;
    file_d_table[1].file_type = 3;
}


    /*
    1. PARSE 
        ~ PARSE IS DONE
    2. EXE CHECK
        ~ LINE 82 in filesys.c -- ELF magic # is defined in header (sets flag if match)
    3. PAGING
        ~ Calling set_address helper function to point to correct 4MB offset from 8MB of physcial addr
    4. USER LVL PROGRAM LOADER
        ~ Copying Data, from filesystem using read data style function. Load into 0x8048000        
    5. CREATE PCB
        ~ Fill array of fd tables 
    6. CONTEXT SWITCH
        ~ Zohair 
    */


    /*
    sys_open : 
    -- have to check if file is valid to open 
    -- switch statement to see what type of file is being opened
        -fd = 0 is RTC
    --return fd index

    Sys_close:
    -- check if file has been opened (using flags??)
    -- reset file to close & call corresponding close 

    Sys_write: 
    -- make sure file is in use 
    -- call corresponding write file (using fda)

    Sys_read: 
    -- make sure file is in use 
    -- call corresponding write file (using fda)

    In file array need:
        -file operation
        -flags
        -idk what else yet
    */
