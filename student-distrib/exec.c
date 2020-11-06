#include types.h

int execute(const unint8_t *command){
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

   // find a PID 
    int pid = get_pid(); // returns -1 on success
    if(pid == -1){return -1;}
    // fill pcb 
    /*
    https://www.geeksforgeeks.org/process-table-and-process-control-block-pcb/
    PCB 
    esp, ebp
    argument from parse
    fd table 
    pcb is utilized (bool)
    */
}