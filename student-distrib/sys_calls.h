#ifndef SYSCALL_H
#define SYSCALL_H

#include "types.h"



typedef struct fop_struct{
    int32_t (*open)(const uint8_t* filename);
    int32_t (*close)(int32_t fd);
    int32_t (*read)(int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write)(int32_t fd, const void* buf, int32_t nbytes);
} fop;


typedef struct fd_table_struct{
    fop* fop_;
    int file_type;
    int exists; // bool type var: 1 or -1
}fd_table;

fd_table file_d_table[8]; // number of sys calls will be 8








extern void sys_call_handler();
extern int32_t sys_close(int32_t fd);
extern int32_t sys_open(const uint8_t *filename);
extern int32_t sys_read(int32_t fd, void *buf, int32_t nbytes);
extern int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes);



#endif
