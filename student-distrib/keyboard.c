#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "terminal.h"


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
#define ESC_KEY_PRESS 0x1
#define F10_KEY_PRESS 0x44
#define F11_KEY_PRESS 0x57


// this section if for defining sys stuff
//PORT # ON PIC
#define IRQ_KB 1

// Under PS/2 Controller IO Ports
//https://wiki.osdev.org/%228042%22_PS/2_Controller#Data_Port
#define PORT_NUM_KB 0x60
//there are 59 chars we need to account for (0x3B)
#define NUM_OF_CHARS 0X3B
#define FLAG_COUNT 0x04


// based on https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
// where I have NULLs are the specail scan codes, like enter, esc, tab, etc.
char char_array_normal[NUM_OF_CHARS] = {
                                        '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
                                        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\0', '\0', 'a', 's',
                                        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',
                                        'b', 'n', 'm', ',', '.', '/', '\0', '*', '\0', ' ', '\0'
                            };



char char_array_shift_no_cap[NUM_OF_CHARS] = {
                                                '\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
                                                'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\0', '\0', 'A','S',
                                                'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|', 'Z', 'X', 'C',
                                                'V','B', 'N', 'M', '<', '>', '?', '\0', '*', '\0', ' ', '\0'
                                };


char char_array_noshift_cap[NUM_OF_CHARS] = {
                                                '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\0', '\0',
                                                'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\0', '\0', 'A','S',
                                                'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', '\0', '\\', 'Z', 'X',
                                                'C', 'V','B', 'N', 'M', ',', '.', '/', '\0', '*', '\0', ' ', '\0'
                                };


char char_array_shift_cap[NUM_OF_CHARS] = {
                                                '\0', '\0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\0', '\0',
                                                'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\0', '\0', 'a', 's',
                                                'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"', '~', '\0', '|', 'z', 'x', 'c',
                                                'v', 'b', 'n', 'm', '<', '>', '?', '\0', '*', '\0', ' ', '\0'
                                };

/*
FLAG MAP
Flag[0]: Shift
Flag[1]: Caps_lock
Flag[2]: Control
Flag[3]: Alt
*/
int flag_keys[FLAG_COUNT] = {0, 1, 0, 0};



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
    enable_irq(IRQ_KB);
}


/*
 NAME: keyboard_handler
 DESCRIPTION: reads from keyboard and displays the chars
 INPUTS: none (inb from lib.c is used)
 OUTPUTS: none
 RETURN VALUE: none
 IMPACTS ON OTHERS: values and chars displayed onto the screen
 */
void keyboard_handler() {
    unsigned int scan_code;
    char key_char;
    int x_pos;
    int y_pos;
    // cli();
    scan_code = inb(PORT_NUM_KB);
/*
        
|===================================================|
|					Magic Numbers					|
|===================================================|

    0x2A & 0x36 ==> Press Shift
    0x3A        ==> Press CapsLock
    0x1D        ==> Press Control
    0x38        ==> Press Alt
    0xAA & 0xB6 ==> Release Shift
    0x9D        ==> Release Control 
    0xB8        ==> Release Alt

            FLAG MAP
    Flag[0]: Shift
    Flag[1]: Caps_lock
    Flag[2]: Control
    Flag[3]: Alt
*/


    // PRESS
    if(scan_code == 0x2A || scan_code == 0x36){flag_keys[0]=1;} // SHIFT
    // CAPS only changes states on second press, thus, we dont care about release
    if(scan_code == 0x3A){flag_keys[1] = ~flag_keys[1];}  //CAPS LOCK
    if(scan_code == 0x1D){flag_keys[2]=1;}  // CONTROL
    if(scan_code == 0x38){flag_keys[3]=1;}  // ALT
    
    // RELEASE
    if(scan_code == 0xAA || scan_code == 0xB6){flag_keys[0]=0;} // SHIFT
    if(scan_code == 0x9D){flag_keys[2]=0;}  // CONTROL
    if(scan_code == 0xB8){flag_keys[3]=0;}  // ALT
    if(char_count<127){
        if((scan_code>ESC_KEY_PRESS && scan_code <= F10_KEY_PRESS) || (scan_code == F11_KEY_PRESS)){
            int case_key;
            /*
                CASES
            CASE 0: NORMAL
            CASE 1: SHIFT
            CASE 2: CAPS
            CASE 3: CAPS + SHIFT
            CASE 4: CTRL 
            CASE 5: ALT
            */
            if(flag_keys[0]== 0 && flag_keys[1]== 1){case_key=0;} // NORMAL
            if(flag_keys[0]== 1 && flag_keys[1]== 1){case_key=1;} // SHIFT
            if(flag_keys[0]== 0 && flag_keys[1]== -2){case_key=2;} // CAPS
            if(flag_keys[0]== 1 && flag_keys[1]== -2){case_key=3;} // SHIFT + CAPS
            //if(flag_keys[2]==1){printf("CONTROL IS PRESSED");}
            //if(flag_keys[3]==1){printf("ALT IS PRESSED");}
            //Select the correct array 
            if(case_key==0){key_char = char_array_normal[scan_code];}
            if(case_key==1){key_char = char_array_shift_no_cap[scan_code];}
            if(case_key==2){key_char = char_array_noshift_cap[scan_code];}
            if(case_key==3){key_char = char_array_shift_cap[scan_code];}
            // special_key_check checks if any of the special keys have been pressed
            // returns 1 if that is the case, thus, it wont print that char 
            // Scan code 0x0E is Backspace
            if((special_key_check(scan_code)!=1) && (scan_code != 0x0E) && (scan_code != 0x1C)){
                putc(key_char);
                keyboard_buffer[kb_idx]=key_char;
                keyboard_buffer[kb_idx+1]='\n';
                ++kb_idx;
                ++char_count;
            }
        }
    }
    // Scan code 0x0E is Backspace
    if(scan_code==0x0E && kb_idx > 0){
        rm_c();
        // keyboard_buffer[kb_idx]='\n';
        kb_idx--; 
        char_count--;
        keyboard_buffer[kb_idx]='\n';

    }

    

    //CTRL+L Handling
    // L ==> 0x26
    // flag_keys[2] holds state of control key (1 is pressed down)
    if((scan_code==0x26) && (flag_keys[2]==1)){
        clear();
        x_pos=0;
        y_pos=0;
        update_cursor(x_pos, y_pos);
    }

    if((scan_code == 0x0F) && (kb_idx<127)){
        putc(' ');
        keyboard_buffer[kb_idx]=' ';
        ++kb_idx;
        ++char_count;
    }


    //Enter Handling 0x1C
    if(scan_code == 0x1C){
        putc('\n');
        enter_p_flag = 1;
        kb_idx = 0;
        char_count = 0;
    }
    send_eoi(IRQ_KB);
    // sti();
}




/*
|===================================================|
|				Helper Functions		    		|
|===================================================|
*/


/*
 NAME: special_key_check
 DESCRIPTION: checks if key press was a special key (shift, caps, etc)
 INPUTS: scan_code
 OUTPUTS: none
 RETURN VALUE: true: 1 false: 2
 IMPACTS ON OTHERS: Decides if char gets printed or not
 */
// true: 1 false: 2
int special_key_check(unsigned int code){
    if(code == 0x01){return 1;} // esc
    if(code == 0x2A){return 1;} // left shift
    if(code == 0x36){return 1;} // right shift
    if(code == 0x1D){return 1;} // control
    if(code == 0x37){return 1;} // keypad *
    if(code == 0x38){return 1;} // left alt
    if(code == 0x3A){return 1;} // CAPs lock
    if(code == 0x3B){return 1;} // F1
    if(code == 0x3C){return 1;} // F2
    if(code == 0x3D){return 1;} // F3
    if(code == 0x3E){return 1;} // F4
    if(code == 0x3F){return 1;} // F5
    if(code == 0x40){return 1;} // F6
    if(code == 0x41){return 1;} // F7
    if(code == 0x42){return 1;} // F8
    if(code == 0x43){return 1;} // F9
    if(code == 0x44){return 1;} // F10
    if(code == 0x8E){return 1;} // Backspace
    if(code == 0x0E){return 1;} // Backspace
    if(code == 0x0F){return 1;} // tab press
    if(code == 0x8F){return 1;} // tab release
    return 2;
}

