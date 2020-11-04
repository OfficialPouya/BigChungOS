#include "memory.h"
#include "lib.h"
#include "terminal.h"
#include "sys_calls.h"
#include "FileSystem.h"

void sys_call_handler() {
    printf("\nBad System Call\n");
}

int32_t sys_open(const uint8_t *filename) {
    if (filename[0] == 0) {return -1;}
    int f_o = file_open(filename);
    if(f_o == -1){return -1;}
    // insert into file directory table
    // return where in the fdt we inserted 

    return 1; // just for now
}

int32_t sys_close(int32_t fd) {
    if (fd <= 2 || fd >= 6) {return -1;}
    // remove from dft
    // return fail or success from remove
}

int32_t sys_write(int32_t fd, const void *buf, int32_t nbytes) {
    if(!buf){return -1;}
    // the fd ranges from 0 - 6
    if (fd <= 0 || fd >= 6) {return -1;}
    if(fd==1){return terminal_write(fd, buf, nbytes);}
    else{
        // not sure
    }

}


int32_t sys_read(int32_t fd, void *buf, int32_t nbytes) {
    if (!buf){return -1;}
    if (fd < 0 || fd >= 6 || fd == 1) {return -1;}
    if (fd == 0) {return terminal_read(fd, buf, nbytes);}
    else {
        // not sure
    }
}
