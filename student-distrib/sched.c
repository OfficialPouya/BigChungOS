#include "sched.h"



void init_PIT(uint32_t freq){
    // stuff -[paul]
    return;
}

/*
 NAME: pit_handler
 DESCRIPTION: keeps tracks of interrupts and spawn new shells at 0, 1, 2 interrupts
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: Opens New shells
 */
void pit_handler(){
    /*
    * check which terminal to go to 

    if(terminals[curr_terminal].pid_shell == 0)  <-- no programs are running in terminal 
        * then start shell


    else 
        * switch to next terminal based on curr_terminal
        * restore  
    

    
    * return
    */

    //save the current tss state
    //update the scheduled terminal
    //load the newly scheduled tss
    //page the video memory to write to
    //map into a video buffer (not main)
    return;
}



/*
 NAME: start_terminals
 DESCRIPTION: initlizes stuff for all 3 terminals
 INPUTS:  NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: initlizes stuff for all 3 terminals (sets stuff to 0 or -1)
 */
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
        terminals[idx].curr_idx = 0;
        terminals[idx].screen_start = 0;
        terminals[idx].curr_process = -1;
        memset(terminals[idx].buf_kb, 0, KB_BUFFER_SIZE);
        memset(terminals[idx].video_buffer, 0, KB_FOUR_OFFSET);
    }   
    on_screen = 0;
    curr_terminal = 0;
}


/*
 NAME: switch_terminal
 DESCRIPTION: manages the screen data, kb info, and video mem while switching terminals
 INPUTS:  curr_terminal and target_terminal
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: saves and restore terminal information 
 */
void switch_terminal(uint8_t curr_terminal, uint8_t target_terminal){
    
    // QUICK CHECKS
    if(target_terminal == on_screen) {return;}
    if(target_terminal > NUMBER_OF_TERMINALS) {return;}
    if(curr_terminal != on_screen){curr_terminal=on_screen;}

    /*
    MAGIC NUMBERS
    get_screen_pos hot key values
    get_screen_pos(0) = x
    get_screen_pos(1) = y
    get_kb_info(0) = kb_idx
    get_kb_info(1) = char_count
    */ 

    // SAVE CURR TERMINAL VALUES BACK INTO ITS PROPPER STRUCT
    terminals[curr_terminal].screen_x = get_screen_pos(0);
    terminals[curr_terminal].screen_y = get_screen_pos(1);
    terminals[curr_terminal].curr_idx = get_kb_info(0);
    terminals[curr_terminal].num_chars = get_kb_info(1);
    memcpy(terminals[curr_terminal].video_buffer, (void *) MAIN_VIDEO, KB_FOUR_OFFSET);
    memcpy(terminals[curr_terminal].buf_kb, keyboard_buffer, KB_BUFFER_SIZE);

    // RESTORE TARGET TERMINAL VALUES FROM ITS PROPPER STRUCT
    update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    memcpy((void *) MAIN_VIDEO, terminals[target_terminal].video_buffer, KB_FOUR_OFFSET);
    memcpy(keyboard_buffer, terminals[target_terminal].buf_kb, KB_BUFFER_SIZE);
    // magic num 0xB8 index inside page table
    // page_table1[0xB8] |= MAIN_VIDEO;
    flush_tlb();
    on_screen = target_terminal;
    return;

}

