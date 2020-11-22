#include "sched.h"



/*
 NAME: pit_asm
 DESCRIPTION: calls the C code that stores register values before calling our c function for the pit handler
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: PIT is called everytime it ticks (20 Hz / 50ms)
 */



/*
 NAME: pit_handler
 DESCRIPTION: keeps tracks of interrupts and spawn new shells at 0, 1, 2 interrupts
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: Opens New shells
 */
void pit_handler(){
    // uint32_t esp;
    // uint32_t ebp;
    // //saving parent esp and ebp
    // asm volatile ( 
    // "movl %%esp, %0 \n\
    //      movl %%ebp, %1"
    // :"=r"(esp), "=r"(ebp) //outputs
    // : //input operands
    // :"memory" //clobbers
    // ); 
    //what we have to do after

    //save the current tss state
    //update the scheduled terminal
    //load the newly scheduled tss
    //page the video memory to write to
    //map into a video buffer (not main)
    return;
}

