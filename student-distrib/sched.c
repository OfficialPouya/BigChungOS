#include "lib.h"
#include "i8259.h"
#include "sched.h"
volatile uint32_t pit_count = 0; // counter for our 3 initial shells


void init_PIT(uint32_t freq){
    uint32_t div;
    uint8_t lower;
    uint8_t higher;
    div = MAX_PIT_FREQ / freq;
    lower = div & 0xFF;     // get lowest byte
    higher = (div >> 8) & 0xFF;     // get highest byte
    test_val = 1; // used to see if pit was working
    // https://wiki.osdev.org/Programmable_Interval_Timer
    outb(ACCESS_LO_HI | OP_SQUARE_WAVE, MODE_THREE_SELECT); // x43 for mode 3, 0x30 gives us access mode, 0x6 is
                          // operating mode (square wave generator)
    outb(lower, CHANNEL_ZERO_PORT);      // send low byte to channel 0
    outb(higher, CHANNEL_ZERO_PORT);  // send high byte of div to channel 0
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
void pit_handler(void){
    uint32_t esp;
    uint32_t ebp;
    asm volatile(
        "movl %%ebp, %0;"
        "movl %%esp, %1"
    : "=r"(ebp), "=r"(esp)
    :
    );
    // terminals[curr_terminal].save_tss = tss; // this is wasteful, just for now ;)
    all_pcbs[terminals[curr_terminal].ebp[terminals[curr_terminal].curr_process]].ebp_task = ebp;
    all_pcbs[terminals[curr_terminal].esp[terminals[curr_terminal].curr_process]].esp_task = esp;

    terminals[curr_terminal].screen_x = get_screen_pos(0);
    terminals[curr_terminal].screen_y = get_screen_pos(1);  
    
    curr_terminal = (curr_terminal+1)%NUMBER_OF_TERMINALS;


    if(pit_count<NUMBER_OF_TERMINALS){
        // switch_terminal_work(pit_count);
        curr_terminal = pit_count;
        switch_terminal_work(curr_terminal);
        terminals[curr_terminal].screen_x = get_screen_pos(0);
        terminals[curr_terminal].screen_y = get_screen_pos(1);
        
        // might be able to replace the above with less logic
        pit_count++;
        printf("Terminal %d\n", pit_count);
        send_eoi(0); // PIT IRQ is 0
        sys_execute((uint8_t*)"shell");
    }

    else if (pit_count == NUMBER_OF_TERMINALS){
        curr_terminal = 0;
        switch_terminal_work(0);
        // printf("pit_count: %d", pit_count);
        pit_count++;
    }
    
    
    // tss.ss0 = KERNEL_DS;
    // tss = terminals[curr_terminal].save_tss;
    tss.esp0 = 0x800000 - ((terminals[curr_terminal].procs[terminals[curr_terminal].curr_process])*4096*2)-4;
    ebp = all_pcbs[terminals[curr_terminal].ebp[terminals[curr_terminal].curr_process]].ebp_task;
    esp = all_pcbs[terminals[curr_terminal].esp[terminals[curr_terminal].curr_process]].esp_task;

    // NOT CORRECT!
    // update_user_addr(terminals[curr_terminal].procs[terminals[curr_terminal].curr_process]);

    if(curr_terminal == on_screen){
        terminals[curr_terminal].video_buffer = (char *)MAIN_VIDEO;// map into the main video buffer
        // switch(curr_terminal) {
        //     case 0: // save to term 0
        //         memcpy((uint8_t *)MAIN_VIDEO,(uint8_t *) TERM0, 4096);
        //         break;
        //     case 1: // save to term 1
        //         memcpy((uint8_t *)MAIN_VIDEO,(uint8_t *) TERM1, 4096);
        //         break;
        //     case 2: // save to term 2
        //         memcpy((uint8_t *)MAIN_VIDEO,(uint8_t *) TERM2, 4096);
        //         break;
        // }

    }
    else{
        switch(curr_terminal) {
            case 0: // save to term 0
                terminals[curr_terminal].video_buffer = (char *)TERM0;
                break;
            case 1: // save to term 1
                terminals[curr_terminal].video_buffer = (char *)TERM1;
                break;
            case 2: // save to term 2
                terminals[curr_terminal].video_buffer = (char *)TERM2;
                break;
        }
    }

    asm volatile(
        "movl %0, %%ebp;"
        "movl %1, %%esp"
    :
    : "r"(ebp), "r"(esp)
    );

    // switch_terminal_work(curr_terminal);
    
    send_eoi(0);
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
void start_terminals(void){
    int idx;
    on_screen = 0;

    for(idx=0; idx<NUMBER_OF_TERMINALS; idx++){
        terminals[idx].screen_x = 0;
        terminals[idx].screen_y = 0;
        terminals[idx].curr_idx = 0;
        terminals[idx].enter_p_flag = 0;
        terminals[idx].screen_start = 0;
        terminals[idx].curr_process = 0;
        switch(idx) {
            case 0:
                terminals[idx].video_buffer = (char *)TERM0;
                break;
            case 1:
                terminals[idx].video_buffer = (char *)TERM1;
                break;
            case 2:
                terminals[idx].video_buffer = (char *)TERM2;
                break;
        }

        // add pids per counter array
        memset(terminals[idx].buf_kb, 0, KB_BUFFER_SIZE);
        // memset(terminals[idx].procs, 0, PCB_SIZE)
    }
    pit_counter = 0;
    curr_terminal = 0;
    return;
}


/*
 NAME: switch_terminal
 DESCRIPTION: manages the screen data, kb info, and video mem while switching terminals
 INPUTS:  curr_terminal and target_terminal
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: saves and restore terminal information
 */
void switch_terminal_work(int target_terminal){
    // QUICK CHECKS + FIX
    if(target_terminal >= NUMBER_OF_TERMINALS) return;

    terminals[on_screen].screen_x = get_screen_pos(0);
    terminals[on_screen].screen_y = get_screen_pos(1);    
    
    /*
    MAGIC NUMBERS
    get_screen_pos hot key values
    get_screen_pos(0) = x
    get_screen_pos(1) = y
    */
    update_screen_axis(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    //curr_terminal = target_terminal;

    // memcpy(terminals[on_screen].video_buffer, (void *)MAIN_VIDEO, 4096);

    // memcpy((void *)MAIN_VIDEO ,terminals[on_screen].video_buffer, 4096);

    
    return;
}

/*
 NAME: switch_terminal_keypress
 DESCRIPTION: switches viewed terminal
 INPUTS:  curr_terminal and target_terminal
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: points the video bif to correct, and saves and restores vid of rother terminals
 */
void switch_terminal_keypress(int target_terminal){
    if (target_terminal == on_screen || target_terminal >= NUMBER_OF_TERMINALS) return;

    terminals[on_screen].screen_x = get_screen_pos(0);
    terminals[on_screen].screen_y = get_screen_pos(1);

    switch(on_screen) {
        case 0: // save to term 0
            memcpy((uint8_t *)TERM0, (uint8_t *)MAIN_VIDEO, 4096);
            terminals[on_screen].video_buffer = (char *)TERM0;
            break;
        case 1: // save to term 1
            memcpy((uint8_t *)TERM1, (uint8_t *)MAIN_VIDEO, 4096);
            terminals[on_screen].video_buffer = (char *)TERM1;
            break;
        case 2: // save to term 2
            memcpy((uint8_t *)TERM2, (uint8_t *)MAIN_VIDEO, 4096);
            terminals[on_screen].video_buffer = (char *)TERM2;
            break;
    }
    
    switch(target_terminal) {
        case 0: // switch to term 0
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM0, 4096);
            // terminals[target_terminal].video_buffer = (char *)MAIN_VIDEO;
            break;
        case 1: // switch to term 1
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM1, 4096);
            // terminals[target_terminal].video_buffer = (char *)MAIN_VIDEO;
            break;
        case 2: // switch to term 2
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM2, 4096);
            // terminals[target_terminal].video_buffer = (char *)MAIN_VIDEO;
            break;
    }
    memcpy(terminals[on_screen].buf_kb, keyboard_buffer, KB_BUFFER_SIZE);
    memcpy(keyboard_buffer, terminals[target_terminal].buf_kb, KB_BUFFER_SIZE);

    terminals[on_screen].curr_idx = kb_idx;
    kb_idx = terminals[target_terminal].curr_idx;

    on_screen = target_terminal;
    // curr_terminal = on_screen;
    update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    // check PUTC for cursor for terminal 2 and 3
    return;
}
