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
    if(pit_count<NUMBER_OF_TERMINALS){
        // switch_terminal_work(pit_count);
        curr_terminal = pit_count;

        terminals[curr_terminal].screen_x = 0;
        terminals[curr_terminal].screen_y = 0;
        update_screen_axis(terminals[curr_terminal].screen_x, terminals[curr_terminal].screen_y);

        asm volatile(
            "movl %%ebp, %0;"
            "movl %%esp, %1"
        : "=r"(terminals[curr_terminal].ebp[terminals[curr_terminal].curr_process]), "=r"(terminals[curr_terminal].esp[terminals[curr_terminal].curr_process])
        :
        );

        if(on_screen == curr_terminal){
            page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
            page_table1[(VIDMEM>>ENTRY4KB)] |= MAIN_VIDEO;
            flush_tlb();
        }

        else{
            switch(curr_terminal) {
                case 1:
                    page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
                    page_table1[(VIDMEM>>ENTRY4KB)] |= TERM1;
                    flush_tlb();
                    break;
                case 2:
                    page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
                    page_table1[(VIDMEM>>ENTRY4KB)] |= TERM2;
                    flush_tlb();
                    break;    
            }
        }
        
        // might be able to replace the above with less logic
        pit_count++;
        printf("Terminal %d\n", pit_count);
        send_eoi(0); // PIT IRQ is 0
        sys_execute((uint8_t*)"shell");
    }

    else if (pit_count == NUMBER_OF_TERMINALS){
        terminals[0].screen_x = 7;
        terminals[0].screen_y = 2;

        terminals[1].screen_x = 7;
        terminals[1].screen_y = 2;
        
        terminals[2].screen_x = 7;
        terminals[2].screen_y = 2;
        curr_terminal = 2;

        switch_terminal_work(0);
        // printf("pit_count: %d", pit_count);
        pit_count++;
    }

    // ++curr_terminal;
    // curr_terminal = curr_terminal % NUMBER_OF_TERMINALS;
    // //If PIT_COUNT is not at end, just increment
    // else{
    //     // call the scheduling function here
    //     // make sure to write test functions for this checkpoint
    //     switch_terminal_work(pit_count%NUMBER_OF_TERMINALS);
    //     pit_count++;
    // }

    // if (pit_count == 99){
    //     pit_count = 3;
    // }

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
                terminals[idx].video_buffer = (char *)MAIN_VIDEO;
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

    /*
    MAGIC NUMBERS
    get_screen_pos hot key values
    get_screen_pos(0) = x
    get_screen_pos(1) = y
    get_kb_info(0) = kb_idx
    get_kb_info(1) = char_count
    */

    // SAVE CURR TERMINAL VALUES BACK INTO ITS PROPPER STRUCT
    // UNNECESSARY DUE TO CHANGES IN KEYBOARD, TERMINAL, AND LIB
    terminals[curr_terminal].screen_x = get_screen_pos(0);
    terminals[curr_terminal].screen_y = get_screen_pos(1);

    
    // // saving parent ebp and esp
    // asm volatile(
    //     "movl %%ebp, %0;"
    //     "movl %%esp, %1"
    // : "=r"(terminals[curr_terminal].ebp[terminals[curr_terminal].curr_process]), "=r"(terminals[curr_terminal].esp[terminals[curr_terminal].curr_process])
    // :
    // );

    // if (terminals[target_terminal].ebp[terminals[target_terminal].curr_process] != 0){
    //     update_user_addr(terminals[target_terminal].procs[terminals[target_terminal].curr_process]);

    //     // recall ebp and esp
    //     asm volatile(
    //         "movl %0, %%ebp;"
    //         "movl %1, %%esp"
    //     :
    //     : "r"(terminals[target_terminal].ebp[terminals[target_terminal].curr_process]), "r"(terminals[target_terminal].esp[terminals[target_terminal].curr_process])
    //     );
    // }

    // step 1: push general (just needs to be callee saved but all is ok) registers and store ebp and esp
    // asm volatile(
    //     "pushal:"
    //     "movl %%ebp, %0;"
    //     "movl %%esp, %1"
    // : "=r"(terminals[curr_terminal].ebp[terminals[curr_terminal].curr_process]), "=r"(terminals[curr_terminal].esp[terminals[curr_terminal].curr_process])
    // :
    // );

    // step 2: update variables related to current terminal
    // update_screen_axis(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    
    // MISSING:
    // set esp0
    // tss.esp0 = 0x800000 - ((terminals[target_terminal].procs[terminals[target_terminal].curr_process])*4096*2)-4; would just be this right,
    // online suggesting we change it (plus we have esp used elsewhere)
    // tss.ss0 = KERNEL_DS;

    // wtf does esp0 get used for and does it depend on ss0? does it change in the user program? 
    // how does it vary from the esp register?
    // esp0 points to the 8kB the process is in, in order to switch properly we need to point to the proper esp0 (kernel stack?)/
    // thomas recommended storing the esp0 in pcb and restoring it upon switch
    // esp will change bc it's the actual kernel stack, esp0 doesn't? esp0 is a point of reference, not something that should change

    // step 3: switch to esp of next process (ebp as well)
    // step 4: pop off anything left from sched() (shouldn't be anything I think)
    // step 5: restore general registers
    // step 6: iret
    //     asm volatile(
    //         "movl %0, %%ebp;"
    //         "movl %1, %%esp;"
    //         "popal;"
    //         "iret"
    //     :
    //     : "r"(all_pcbs[terminals[target_terminal].procs[terminals[target_terminal].curr_process]]), "r"(all_pcbs[terminals[target_terminal].procs[terminals[target_terminal].curr_process]])
    //     );

    // SCREEN DATA CONTROL
    // DONT NEED TO RESTORE PREVIOUS, JUST CHANGE POINTER TO ADD ONTO EXISTING
    // TWO CASES, WORKED ONE IS ON SCREEN OR NOT
    if(on_screen == target_terminal){
        page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
        page_table1[(VIDMEM>>ENTRY4KB)] |= MAIN_VIDEO;
        flush_tlb();
    }

    else{
        switch(target_terminal) {
            case 0:
                page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
                page_table1[(VIDMEM>>ENTRY4KB)] |= TERM0;
                flush_tlb();
                break;    
            case 1:
                page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
                page_table1[(VIDMEM>>ENTRY4KB)] |= TERM1;
                flush_tlb();
                break;
            case 2:
                page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
                page_table1[(VIDMEM>>ENTRY4KB)] |= TERM2;
                flush_tlb();
                break;    
        }
    }

    // memcpy(terminals[on_screen].buf_kb, keyboard_buffer, KB_BUFFER_SIZE);
    // memcpy(keyboard_buffer, terminals[target_terminal].buf_kb, KB_BUFFER_SIZE);

    // SCREEN DATA CONTROL
    // DONT NEED TO RESTORE PREVIOUS, JUST CHANGE POINTER TO ADD ONTO EXISTING
    // TWO CASES, WORKED ONE IS ON SCREEN OR NOT
    // update_screen_axis(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);

    // save current esp
    // save current ebp
    // update_user_addr(target pid);
    // restore target esp
    // restore target ebp

    // change_vid_mem(target_terminal);
    // if(on_screen == target_terminal){
    //     update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    // }
    // else{
    update_screen_axis(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    // }

    curr_terminal = target_terminal;

    // RESTORE TARGET TERMINAL VALUES FROM ITS PROPPER STRUCT
    // update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    // update_screen_axis(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
    // not needed after changes to keyboard functionality
    // memcpy(keyboard_buffer, terminals[target_terminal].buf_kb, KB_BUFFER_SIZE);

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
            break;    
        case 1: // save to term 1
            memcpy((uint8_t *)TERM1, (uint8_t *)MAIN_VIDEO, 4096);
            break;
        case 2: // save to term 2
            memcpy((uint8_t *)TERM2, (uint8_t *)MAIN_VIDEO, 4096);
            break;    
    }

    switch(target_terminal) {
        case 0: // switch to term 0
            page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
            page_table1[(VIDMEM>>ENTRY4KB)] |= MAIN_VIDEO;
            flush_tlb(); 
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM0, 4096);
            break;    
        case 1: // switch to term 1
            page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
            page_table1[(VIDMEM>>ENTRY4KB)] |= MAIN_VIDEO;
            flush_tlb(); 
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM1, 4096);
            break;
        case 2: // switch to term 2
            page_table1[(VIDMEM>>ENTRY4KB)] &= 0xFFF;      // Save all lower 12 bits
            page_table1[(VIDMEM>>ENTRY4KB)] |= MAIN_VIDEO;
            flush_tlb(); 
            memcpy((uint8_t *)MAIN_VIDEO, (uint8_t *)TERM2, 4096);
            break;    
    }
    memcpy(terminals[on_screen].buf_kb, keyboard_buffer, KB_BUFFER_SIZE);
    memcpy(keyboard_buffer, terminals[target_terminal].buf_kb, KB_BUFFER_SIZE);

    terminals[on_screen].curr_idx = kb_idx;
    kb_idx = terminals[target_terminal].curr_idx;

    on_screen = target_terminal;
    // curr_terminal = on_screen;
    update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);

    return;
}
