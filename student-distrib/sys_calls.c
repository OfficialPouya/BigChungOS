#include "sys_calls.h"

int fd_index_holder;

fop rtc_struct = {rtc_open, rtc_close, rtc_read, rtc_write};
fop file_struct = {file_open, file_close_helper, file_read_helper, file_write};
fop dir_struct = {dir_open, dir_close, dir_read, dir_write};
fop terminal_struct = {terminal_open, terminal_close, terminal_read, terminal_write};

/*
 NAME: sys_call_handler
 DESCRIPTION: just prints statement
 INPUTS: NONE
 OUTPUTS: "\nBad System Call\n"
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: NONE
 */
void sys_call_handler() {
    printf("\nBad System Call\n");
}

/*
 NAME: sys_open
 DESCRIPTION: Attempts to open a file from the filename that was passed, holding the spot and passing it
 INPUTS: filename
 OUTPUTS: NONE
 RETURN VALUE: -1 on fail, index_holder on success
 IMPACTS ON OTHERS: changes fdt, pcb
 */
int32_t sys_open(const uint8_t *filename) {
    dentry_t temp_dentry;
    int t_index, floop;
    // be able to see if file exists, if not return -1
    //printf("here 1 %d \n", curr_dentry->filetype);
    if(read_dentry_by_name(filename, &temp_dentry) == -1) return -1;
    //printf("here 2 %d \n", curr_dentry->filetype);
    // since exe & halt are index 0, 1 we start at 2, and there are 8 total calls
    for(t_index=2; t_index<MAX_FD_AMNT; t_index++){
        // used : 1
        // empty : -1
        if(all_pcbs[pid_counter].fdt[t_index].exists == -1){
            // set struct items
            all_pcbs[pid_counter].fdt[t_index].exists = 1;
            all_pcbs[pid_counter].fdt[t_index].file_type = temp_dentry.filetype;
            for (floop = 0; floop< FILENAME_LEN; floop++){
                all_pcbs[pid_counter].fdt[t_index].filename[floop] = temp_dentry.filename[floop];
            }
            switch (temp_dentry.filetype){
                case 0:
                    all_pcbs[pid_counter].fdt[t_index].fop_ = &rtc_struct;
                    break;
                case 1:
                    all_pcbs[pid_counter].fdt[t_index].fop_ = &dir_struct;
                    break;
                case 2:
                    all_pcbs[pid_counter].fdt[t_index].fop_ = &file_struct;
                    break;
            }
            return t_index;
        }
    }
    return -1; // nothing was able to get added
}

/*
 NAME: sys_close
 DESCRIPTION: closes the fd
 INPUTS: fd
 OUTPUTS: NONE
 RETURN VALUE: -1 Fail, 0 on success
 IMPACTS ON OTHERS:
 */
int32_t sys_close(int32_t fd) {
    if (fd <= 1 || fd >= MAX_FD_AMNT) return -1;
    if(all_pcbs[pid_counter].fdt[fd].exists == 1){
        all_pcbs[pid_counter].fdt[fd].exists = -1;
        return 0;
    }
    else return -1;
}

/*
 NAME: sys_write
 DESCRIPTION: Writes 'nbytes' bytes from 'buf' into the file associated with 'fd'
 INPUTS: 'nbytes' bytes into 'buf' from the file corresponding to the given 'fd'.
 OUTPUTS: none
 RETURN VALUE: -1 on fail or whatever write returns
 IMPACTS ON OTHERS: changes the pcb in use
 */
int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes) {
    if(!buf) return -1;
    // the fd ranges from 0 - 7
    if (fd <= 0 || fd >= MAX_FD_AMNT) return -1;
    if(all_pcbs[pid_counter].fdt[fd].exists == -1) return -1;
    return all_pcbs[pid_counter].fdt[fd].fop_->write(fd, buf, nbytes);
}

/*
 NAME: sys_read
 DESCRIPTION: 'nbytes' bytes into 'buf' from the file corresponding to the given 'fd'.
 INPUTS:  file descriptor, buffer to read from, number of bytes to read from
 OUTPUTS: NONE
 RETURN VALUE: number of bytes read, -1 on fail
 IMPACTS ON OTHERS: none
 */
int32_t sys_read(int32_t fd, void *buf, int32_t nbytes) {
    if (!buf) return -1;
    if (fd < 0 || fd >= MAX_FD_AMNT || fd == 1 || buf == NULL) return -1;
    if(all_pcbs[pid_counter].fdt[fd].exists == -1) return -1;
    return all_pcbs[pid_counter].fdt[fd].fop_->read(fd, buf, nbytes);
}

/*
 NAME: sys_execute
 DESCRIPTION: executes program
 INPUTS: command
 OUTPUTS: none
 RETURN VALUE: -1 on fail, 0 on OK
 IMPACTS ON OTHERS: changes the pcb in use, changes ebp, eax, tss.ss0, pid_counter
 */

int32_t sys_execute(const uint8_t *command){

    // 1. PARSE (Chloe :: DONE)
    int command_index, i, j; // variables to be used as indices
    int command_len_check = 0;

    // do not want to run more than 6 processes (5 bc -1 indexed)
    if(pid_counter>PCB_SIZE-1){return -1;}
    command_index = 0;
    i = 0;
    j = 0;
    uint8_t filename[FILENAME_LEN] = { 0 }; // array to hold file name

    //1. PARSE COMMAND FOR FILE NAME
    //check for valid command
    if(command == NULL){
        all_pcbs[pid_counter].in_use = -1;
        return -1;
    }

    // iterate through initial white space
    while(command[command_index] == ' '){
        command_index++;
    }

    // get file name from command
    while(command[command_index] != ' ' && command[command_index] != '\0'){
        command_len_check++;
        if (command_len_check > 31){
          printf("filename to execute is too long\n");
          return -1;
        }

        filename[i] = command[command_index];
        i++;
        command_index++;

    }

    // add null char at end of file name
    filename[i] = '\0';

    //iterate through second set of white space before args
    while(command[command_index] == ' '){
        command_index++;
    }
    command_len_check = 0;
    // stores args in pcb
    while(command[command_index] != ' ' && command[command_index] != '\0'){
        //store args in pcb

        all_pcbs[pid_counter].args[j] = command[command_index];
        j++;
        command_index++;
    }


    // may need to append newline char not positive tho
    // 2. EXE CHECK (PAUL :: Done/Not Checked)
    // 3. PAGING (PAUL :: Done/Not Checked)
    // 4. USER LVL PROGRAM LOADER (PAUL :: Done/Not Checked)
    // helper function in filesys to check first 4 bytes

    int fd;
    int32_t eip_data;
    if(-1 != (eip_data = exec_check(filename))){
        ++pid_counter;
        init_pcb(pid_counter);
        // then do what needs to be done with exec
        //paging, call change address fucntion
        update_user_addr(pid_counter); // put process number here, will change pointer to correct page

        // laodeer, load bytes to right address
        fd = sys_open(filename);
        // num 0x8048000: given, starting addr
        // num 4MB = 4096 * 1024 (at most or whatever file size is) 
        sys_read(fd, (void*)0x8048000, 4096*1024);
    }
    else{
        return -1;
    }

    // 6.  CONTEXT SWITCH (Zohair)

    // the math: 8MB - (curr pid)*8KB-4B
    tss.esp0 = 0x800000 - ((pid_counter)*4096*2)-4;
    tss.ss0 = KERNEL_DS;

        asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1"
        :"=r"(all_pcbs[pid_counter].old_ebp), "=r"(all_pcbs[pid_counter].old_esp)
    );

    asm volatile (
        "pushl %0;"
        "movl %0, %%ebx;"
        "movw %%bx, %%ds;"
        "pushl %1;"
        "pushfl;"
        "popl %%ebx;"
        "orl $0x200, %%ebx;"
        "pushl %%ebx;"
        "pushl %3;"
        "pushl %2;"
        "iret;"
        "from_halt:;"
        "leave;"
        "ret"
        :
        : "r" (USER_DS), "r" (0x83FFFFC), "r" (eip_data), "r" (USER_CS)
        : "ebx", "memory"
    );

    return 0;
}

/*
 NAME: sys_halt
 DESCRIPTION: this is to halt the user program, doesnt allow you to halt the base shell
 INPUTS: status
 OUTPUTS: none
 RETURN VALUE: 0 on success
 IMPACTS ON OTHERS: changes the pcb in use, changes ebp, eax, tss.ss0, pid_counter
 */
int32_t sys_halt(uint8_t status){
    uint32_t status_num = (uint32_t) status;
    int fdt_loop;
    for (fdt_loop = 0; fdt_loop < MAX_FD_AMNT; fdt_loop++){
      sys_close(fdt_loop);
    }
    all_pcbs[pid_counter].in_use = -1;
    --pid_counter;



    if(pid_counter==-1){
        //all_pcbs[pid_counter].in_use=-1;
        //printf("Restarting Shell... \n"); //restart the base shell
        sys_execute((uint8_t *) "shell");
        return 0;
    }
    update_user_addr(pid_counter);
    tss.ss0 = KERNEL_DS;
    /*
    Magic Numbers
    number '0x800000':
    number '1':         this is bc 0 * anything will be 0
    number '4096':      4KB
    number '2':         to get to 8KB
    */

    // the math: 8MB - (curr pid)*8KB-4B
    tss.esp0 = 0x800000 - ((pid_counter)*4096*2)-4;
    asm volatile (
        "movl %0, %%ebp;"
        "movl %1, %%esp;"
        "movl %2, %%eax;"
        "jmp from_halt"
        :
        :"r"(all_pcbs[pid_counter+1].old_ebp), "r"(all_pcbs[pid_counter+1].old_esp) ,"r" (status_num)
    );
    return 0;
}

/*
 NAME: init_pcb
 DESCRIPTION: inits pcb struct
 INPUTS:  curr_pcb (index into pcb array)
 OUTPUTS: NONE
 RETURN VALUE: NONE (void)
 IMPACTS ON OTHERS: Initalizes PCB and its objects
 */
void init_pcb(int curr_pcb){
    int fdt_index;
    all_pcbs[curr_pcb].in_use = 1;
    // only 6 processes 
    for(fdt_index=0; fdt_index < MAX_FD_AMNT; fdt_index++){
        all_pcbs[curr_pcb].fdt[fdt_index].exists = -1;
    }

    // the size of our array of args is 1024
    for (fdt_index = 0; fdt_index < NUM_ARGS; fdt_index++){
        all_pcbs[curr_pcb].args[fdt_index] = 0;
    }

    // stdin members
    all_pcbs[curr_pcb].fdt[0].fop_ = &terminal_struct;
    all_pcbs[curr_pcb].fdt[0].file_type = 3;
    all_pcbs[curr_pcb].fdt[0].exists = 1;
    all_pcbs[curr_pcb].fdt[0].fop_->open((uint8_t*)"blah");

    // stdout members
    all_pcbs[curr_pcb].fdt[1].fop_ = &terminal_struct;
    all_pcbs[curr_pcb].fdt[1].file_type = 3;
    all_pcbs[curr_pcb].fdt[1].exists = 1;
    all_pcbs[curr_pcb].fdt[1].fop_->open((uint8_t*)"blah");
}

/*
 NAME: file_read_helper
 DESCRIPTION: helps out the file reader
 INPUTS:  prototype file reader args, fd#, buf, and bytes to read
 OUTPUTS: NONE
 RETURN VALUE: NONE (void)
 IMPACTS ON OTHERS: passes more useful args to file read
 */
int32_t file_read_helper(int32_t fd, void* buf, int32_t nbytes){
    if (all_pcbs[pid_counter].fdt[fd].exists == 1){
        // call actual file reading func
        return file_read(all_pcbs[pid_counter].fdt[fd].filename, buf, nbytes);
    }

    else return -1;
}

/*
 NAME: file_close_helper
 DESCRIPTION: closes files
 INPUTS:  clsoes files
 OUTPUTS: NONE
 RETURN VALUE: NONE (void)
 IMPACTS ON OTHERS: updates fd table much easier
 */
int32_t file_close_helper(int32_t fd){
    if (all_pcbs[pid_counter].fdt[fd].exists == 1){
        all_pcbs[pid_counter].fdt[fd].exists = -1;
        return 0;
    }

    // file not open
    return -1;
}

/*
 NAME: sys_getargs
 DESCRIPTION: get arguments for programs like cat
 INPUTS:  file descriptor, buffer to read from, number of bytes to read from
 OUTPUTS: NONE
 RETURN VALUE: -1 for now
 IMPACTS ON OTHERS: none
 */
int32_t sys_getargs(uint8_t *buf, int32_t nbytes){
    int floop;
    if (all_pcbs[pid_counter-1].args[0] == '\0') return -1;
    memcpy (buf, all_pcbs[pid_counter-1].args, nbytes);
    for (floop = 0; floop < FILENAME_LEN; floop++){
        all_pcbs[pid_counter-1].args[floop] = '\0';
    }
    return 0;
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
