#ifndef SYSCALL_H
#define SYSCALL_H

#include "types.h"


extern void sys_call_handler();
extern int32_t sys_close(int32_t fd);
extern int32_t sys_open(const uint8_t *filename);
extern int32_t sys_read(int32_t fd, void *buf, int32_t nbytes);
extern int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes);



#endif
