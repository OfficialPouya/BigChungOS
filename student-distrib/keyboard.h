#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "sched.h"

// only type 127 chars per line, the extra space is
// for NULL padding
#define TYPE_BUFFER_SIZE 128
#define MAX_TYPE 127

char keyboard_buffer[TYPE_BUFFER_SIZE];
// kb_idx, is the index for our keyboard buffer
int kb_idx;
// char_count is to ensure we don't go over the char limit
// there will be if statements later on
int char_count;
int keypress_to_vid_flag;

// initialize keyboard
void init_keyboard();
// check if a non-char key has been pressed
int special_key_check(unsigned int code);
// handle keyboard I/O
void keyboard_handler();
int get_kb_info(int arg);
#endif
