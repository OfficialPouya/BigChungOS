#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "terminal.h"

/*
 NAME: terminal_open
 DESCRIPTION: Opens terminal
 INPUTS: filename
 OUTPUTS: none
 RETURN VALUE: 0
 IMPACTS ON OTHERS: accesses a file
 */
//open
int32_t terminal_open(const uint8_t *filename) {
    return 0;
}

/*
 NAME: terminal_close
 DESCRIPTION: cloeas terminal
 INPUTS: filename
 OUTPUTS: none
 RETURN VALUE: 0
 IMPACTS ON OTHERS: ??
 */
// close
int32_t terminal_close(int32_t fd) {
    return 0;
}


/*
 NAME: terminal_read
 DESCRIPTION: reads the kb buffer into another buffer, then clears kb buffer
 INPUTS: fd, buf, #bytes
 OUTPUTS: none
 RETURN VALUE: how many chars it read
 IMPACTS ON OTHERS: clears keyboard buffer and over writes buffer that was passed
 */
//read
int32_t terminal_read(int32_t fd, void *buf, int32_t nbytes) {
    // sti();
    while(enter_p_flag == 0);
    int count = 0;
    int how_many = 0;
    if(nbytes==0){return how_many;}
    if(keyboard_buffer[0]=='\n'){return how_many;}    
    
    while(keyboard_buffer[count] != '\0'){
        ++count;
    }
    if(count == nbytes){
        how_many = count;
    }
    else if(count>nbytes){
        how_many=nbytes;
        }
    else{
        how_many=count;
    }
    //printf("Number of bytes typed: %d \n", how_many);
    memcpy(buf, keyboard_buffer, how_many+1); // copy keyboard buffer into buf
    // set NULL values for the entire keyboard buffer now that the buffer was copied 
    // The value of 128 is the size of the keyboard_buffer
    enter_p_flag = 0;
    kb_idx = 0;
    return how_many;
}



/*
 NAME: terminal_write
 DESCRIPTION: writes what was in buffer into the video mem 
 INPUTS: fd, buf, nbytes
 OUTPUTS: chars onto screen
 RETURN VALUE: count of chars displayed
 IMPACTS ON OTHERS: Changes screen
 */
//write
int32_t terminal_write(int32_t fd, const void *buf, int32_t nbytes) {
    int i = 0;
    int count = 0;  
    // wrtie nbytes into this buf
    for (i = 0; i < nbytes; i++) {
        putc((*(uint8_t *) ((uint32_t) buf + i)));
        ++count;
    }

    // printf("HERE");
    memset(keyboard_buffer, '\0', 128); 
    kb_idx = 0;
    // printf(" HERE after mem set %d \n", count);
    return count;
}

