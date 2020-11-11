#pragma once

#ifndef SYS_CALLS_H
#define SYS_CALLS_H


#include "types.h"
#include "rtc.h"
#include "terminal.h"
#include "lib.h"
#include "FileSystem.h"
#include "memory.h"
#include "x86_desc.h"

#define MAX_FD_AMNT    8 

int pid_counter;
int32_t ret_val;

extern void sys_call_handler();
extern int32_t sys_close(int32_t fd);
extern int32_t sys_open(const uint8_t *filename);
extern int32_t sys_read(int32_t fd, void *buf, int32_t nbytes);
extern int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes);
extern int32_t sys_halt(uint8_t status);
extern int32_t sys_execute(const uint8_t *command);
extern int32_t sys_getargs(uint8_t *buf, int32_t nbytes);

int32_t file_read_helper(int32_t fd, void* buf, int32_t nbytes);
int32_t file_close_helper(int32_t fd);

/*
FOP 
*/
typedef struct fop{
    int32_t     (*open)(const uint8_t* filename);
    int32_t     (*close)(int32_t fd);
    int32_t     (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t     (*write)(int32_t fd, const void* buf, int32_t nbytes);
} fop;

/*
File Descriptor Table
*/
typedef struct fd_table{
    fop*        fop_;
    int32_t     file_type;
    uint8_t     filename[FILENAME_LEN];
    int         exists; // bool type var: 1 or -1
}fd_table;

/*
PCB Struct 
*/
typedef struct pcb{
    int32_t old_esp;
    int32_t old_eip;
    int32_t old_ebp;
    int in_use;
    fd_table fdt[MAX_FD_AMNT]; // we have 8 descriptor tables
    uint8_t args[1024]; // max number of arguments
} pcb;

// pcb first_pcb;
// pcb second_pcb;
void init_pcb(int curr_pcb);

/*
Array of PCBs to keep track of them  
*/
pcb all_pcbs[6]; // we have 0-5


#endif
