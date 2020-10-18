#ifndef KEYBOARD_H
#define KEYBOARD_H

// only type 127 chars per line, the extra space is
// for NULL padding 
#define TYPE_BUFFER_SIZE 128
#define MAX_TYPE 127

char keyboard_buffer[TYPE_BUFFER_SIZE];

// initialize keyboard
void init_keyboard();

// handle keyboard I/O
void keyboard_handler();

#endif