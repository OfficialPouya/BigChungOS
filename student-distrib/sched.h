#pragma once

#ifndef SYS_SCHED_H
#define SYS_SCHED_H

#include "sys_calls.h"
#include "x86_desc.h"

#define KB_BUFFER_SIZE 128

void pit_handler();

typedef struct terminal_t {
    uint8_t screen_x; // screen logical location x
    uint8_t screen_y; // screen logical location y
    uint8_t curr_idx; // current location in command
    int pid_shell;  //current shell that corresponds to this terminal
    uint8_t* video_buffer; //pointer to this terminals video buffer
    uint8_t** screen_start;
    char buf_kb[KB_BUFFER_SIZE];
    char prev_buf[KB_BUFFER_SIZE];
    int curr_process;
    tss_t save_tss;
    uint8_t save_x;
    uint8_t save_y;

} terminal_t;

#endif
