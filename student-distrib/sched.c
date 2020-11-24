#include "sched.h"
volatile uint8_t pit_count = 0; // counter for our 3 initial shells


void init_PIT(uint32_t freq){
    uint32_t div;
    uint8_t lower;
    uint8_t higher;
    div = MAX_PIT_FREQ / freq;
    // paul is doing the rest 
    enable_irq(0); // PIT IRQ num is 0
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
    uint32_t esp;
    uint32_t ebp;
    pit_helper(ebp, esp);
    /*
        * load the new scheduled tss
        * page the video mem that we want to write to  OR  map into the video buff 
    */
    // load the new scheduled tss
    tss = terminals[curr_terminal].save_tss;
    ebp = all_pcbs[terminals[curr_terminal].curr_process].ebp_task;
    ebp = all_pcbs[terminals[curr_terminal].curr_process].ebp_task;

    // page the video mem that we want to write to
    if(curr_terminal == on_screen){
        // map the video page [idk how to do this PAUL]
    }
    else{
        // map into the buffer [idk how to do this PAUL]
    }
    int sc_x;
    int sc_y;
    sc_x = terminals[curr_terminal].screen_x;
    sc_y = terminals[curr_terminal].screen_y;
    update_screen_axis(sc_x, sc_y);
    // map the page [idk how to do this PAUL]

    asm volatile(
        "movl %0, %%ebp;"
        "movl %1, %%esp"
    :
    : "r"(ebp), "r"(esp)
    );
    send_eoi(0); // PIT IRQ is 0
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
    
    // QUICK CHECKS + FIX
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


/*
|===================================================|
|				Helper Functions		    		|
|===================================================|
*/


/*
 NAME: pit_helper
 DESCRIPTION: helps save the task state segment, helps handle pit interrupts
 INPUTS:  esp, ebp
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: changes states of TSS and handles data within struct
 */
void pit_helper(uint32_t ebp,uint32_t esp){
    asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1"
        :"=r"(ebp), "=r"(esp)
    );
    ++interrupt_counter_pit;
    // saving the current state of tss (task state segment)
    terminals[curr_terminal].save_tss = tss;
    all_pcbs[terminals[curr_terminal].curr_process].ebp_task = ebp;
    all_pcbs[terminals[curr_terminal].curr_process].esp_task = esp;

    if(pit_count < NUMBER_OF_TERMINALS){
        switch_terminal('\0',pit_count); // helps restore correct information 
        clear(); // clears whatever was on screen from prev terminal
        terminals[curr_terminal].screen_x = get_screen_pos(0);
        terminals[curr_terminal].screen_y = get_screen_pos(1);
        ++pit_count;
        send_eoi(0); // PIT IRQ is 0
        sys_execute((uint8_t *)"shell");
    }
    else if(pit_count == NUMBER_OF_TERMINALS){
        switch_terminal('\0',0);
        ++pit_count;
    }
    return;
}

