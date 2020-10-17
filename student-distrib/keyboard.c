
//KEYBOARD LAYOUTS IN DIFF MODES, 
// DOESNT INCLUDE ANY ALIGNING OR SPACE CHARS
// NOR ARE WE CONSIDERING NUM PAD

/*
NO SHIFT, NO CAP
` 1 2 3 4 5 6 7 8 9 0 - = 
q w e r t y u i o p [ ] \
a s d f g h j k l ; ' 
z x c v b n m , . /
*/

/*
SHIFT, NO CAP
~ ! @ # $ % ^ & * ( ) _ +
Q W E R T Y U I O P { } |
A S D F G H J K L : "
Z X C V B N M < > ?
*/


/*
NO SHIFT, CAP
` 1 2 3 4 5 6 7 8 9 0 - =
Q W E R T Y U I O P [ ] \
A S D F G H J K L ; ' 
Z X C V B  N M , . /
*/

/*
SHIFT, CAP 
~ ! @ # $ % ^ & * ( ) _ +
q w e r t y u i o p { } |
a s d f g h j k l : "
z x c v b n m < > ?
*/


/*
Things we Need to do:
1. Initialize the Keyboard
2. Keyboard Handler
3. Map the Scan codes 
*/

#include "keyboard.h"
#include "lib.h"
#include "i8259.h"

// All scan codes where found from 
// https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html#ss1.2
// These scan codes are the data from a keyboard comes mainly in the form of 
// scancodes, produced by key presses or used in the protocol with the computer
#define ENTER 0x1C
#define L_CTRL 0x1D
#define L_SHIFT_PRESS 0x2A
#define L_SHIFT_RELEASE 0XAA
#define L_ALT_PRESS 0x38
#define L_AKT_RELEASE 0XB8
#define CTRL_PRESS 0X1D
#define CTRL_RELEASE 0X9D
#define R_SHIFT_PRESS 0X36
#define R_SHIFT_RELEASE 0XB6
#define BACK_SPACE 0X0E

// this section if for defining sys stuff
//PORT # ON PIC
#define IRQ_KB #
//there are 59 chars we need to account for (0x3B)
#define NUM_OF_CHARS 0X3B


// based on https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// where I have 0s are the specail scan codes, like enter, esc, tab, etc....
char char_array_normal[NUM_OF_CHARS] = {            
                                         0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
                                        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's',
                                        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
                                        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
                            };



char char_array_shift_no_cap[NUM_OF_CHARS] = {
                                                0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
                                               'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0, 0, 'A','S',
                                               'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C',
                                               'V','B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
                                };


char char_array_noshift_cap[NUM_OF_CHARS] = {
                                                0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
                                               'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 0, 0, 'A','S',
                                               'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0, '\\', 'Z', 'X',
                                               'C', 'V','B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0
                                };


char char_array_shift_cap[NUM_OF_CHARS] = {
                                              0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
                                             'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', 0, 0, 'a', 's',
                                             'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', '~', 0, '|', 'z', 'x', 'c',
                                             'v', 'b', 'n', 'm', '<', '>', '?', 0, '*', 0, ' ', 0
                                };









/*
 NAME: init_keyboard
 DESCRIPTION: intializes the keyboard 
 INPUTS: none
 OUTPUTS: none
 RETURN VALUE: none
 IMPACTS ON OTHERS: PIC, it will be enabled after this func is called
 */

void init_keyboard() {
    // enable keyboard interrupt on the PIC
    // enable IRQ is in i8259.c
}



