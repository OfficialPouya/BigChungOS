#pragma once

#ifndef SYS_SCHED_H
#define SYS_SCHED_H

#include "sys_calls.h"
#include "x86_desc.h"
#include "keyboard.h"
#define KB_BUFFER_SIZE 128
#define NUMBER_OF_TERMINALS 3
#define KB_FOUR_OFFSET 4096
#define MAIN_VIDEO 0xB8000
#define TERM1_VIDEO (MAIN_VIDEO + KB_FOUR_OFFSET) 
#define TERM2_VIDEO (TERM1_VIDEO + KB_FOUR_OFFSET)
#define TERM3_VIDEO (TERM2_VIDEO + KB_FOUR_OFFSET)
#define max_pit_freq 1193182
void init_PIT(uint32_t freq);
void pit_handler();

uint8_t on_screen; // flag 0,1,2 which terminal should be shown
uint8_t curr_terminal;
typedef struct terminal_t {
    uint8_t screen_x; // screen logical location x
    uint8_t screen_y; // screen logical location y
    uint8_t curr_idx; // current location in command
    int pid_shell;  //current shell that corresponds to this terminal
    uint8_t* video_buffer; //pointer to this terminals video buffer
    uint8_t** screen_start;
    char buf_kb[KB_BUFFER_SIZE];
    int curr_process;
    tss_t save_tss;
    int num_chars;
} terminal_t;

terminal_t terminals[NUMBER_OF_TERMINALS];

void start_terminals();
void switch_terminal(uint8_t curr_terminal, uint8_t target_terminal);
#endif
