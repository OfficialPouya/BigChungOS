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

void start_terminals(){
    terminals[0].video_buffer = (uint8_t *) TERM1_VIDEO;
    memset((void *) TERM1_VIDEO, 0, KB_FOUR_OFFSET);
    terminals[1].video_buffer = (uint8_t *) TERM2_VIDEO;
    memset((void *) TERM2_VIDEO, 0, KB_FOUR_OFFSET);
    terminals[2].video_buffer = (uint8_t *) TERM3_VIDEO;
    memset((void *) TERM3_VIDEO, 0, KB_FOUR_OFFSET);

    int idx;
    for(idx=0; idx<NUMBER_OF_TERMINALS; idx++){
        terminals[idx].pid_shell = idx;
        terminals[idx].screen_x = 0;
        terminals[idx].screen_y = 0;
        terminals[idx].save_x = 0;
        terminals[idx].save_y = 0;
        terminals[idx].curr_idx = 0;
        terminals[idx].screen_start = 0;
        terminals[idx].curr_process = -1;
        memset(terminals[idx].buf_kb, 0, KB_BUFFER_SIZE);
        memset(terminals[idx].video_buffer, 0, KB_FOUR_OFFSET);
    }   
    on_screen = 0;
}

void switch_terminal(uint8_t curr_terminal, uint8_t target_terminal){
    
    // SAVE CURR TERMINAL VALUES BACK INTO ITS PROPPER STRUCT
    /*
    get_screen_pos hot key values
    get_screen_pos(0) = x
    get_screen_pos(1) = y
    */
    terminals[curr_terminal].screen_x = get_screen_pos(0);
    terminals[curr_terminal].screen_y = get_screen_pos(1);
    

    // RESTORE TARGET TERMINAL VALUES FROM ITS PROPPER STRUCT

    flush_tlb();
    on_screen = target_terminal;
    return;

}

