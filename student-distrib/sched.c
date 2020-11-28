#include "lib.h"
#include "i8259.h"
#include "sched.h"
volatile uint32_t pit_count = 0; // counter for our 3 initial shells


void init_PIT(uint32_t freq){
    uint32_t div;
    uint8_t lower;
    uint8_t higher;
    div = MAX_PIT_FREQ / freq;
    lower = div & 0xFF;
    higher = (div >> 8) & 0xFF;
    test_val = 1;
    // https://wiki.osdev.org/Programmable_Interval_Timer
    outb(0x30 | 0x6,0x43); // x43 for mode 3, 0x30 was example given, 0x6 is
                          // channel 0
    outb(lower, 0x40);      // send low byte to channel 0
    outb(higher, 0x40);  // send high byte of div to channel 0
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
    if(pit_count<3){
        switch_terminal_work(pit_count);
        pit_count++;
        printf("Terminal %d\n", pit_count);
        send_eoi(0); // PIT IRQ is 0
        sys_execute((uint8_t*)"shell");
    }

    if (pit_count == 3){
        switch_terminal_work(0);
        pit_count++;
    }

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
        terminals[idx].ProcPerTerm = 0;
        terminals[idx].screen_x = 0;
        terminals[idx].screen_y = 0;
        terminals[idx].curr_idx = 0;
        terminals[idx].screen_start = 0;
        terminals[idx].curr_process = -1;
        // add pids per counter array
        memset(terminals[idx].buf_kb, 0, KB_BUFFER_SIZE);
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
    // terminals[curr_terminal].screen_x = get_screen_pos(0);
    // terminals[curr_terminal].screen_y = get_screen_pos(1);
    // terminals[curr_terminal].curr_idx = get_kb_info(0);
    // terminals[curr_terminal].char_count = get_kb_info(1);
    // memcpy(terminals[curr_terminal].buf_kb, keyboard_buffer, KB_BUFFER_SIZE);

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

    curr_terminal = target_terminal;

    // RESTORE TARGET TERMINAL VALUES FROM ITS PROPPER STRUCT
    update_screen(terminals[target_terminal].screen_x, terminals[target_terminal].screen_y);
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
    if (target_terminal == on_screen) return;

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
    on_screen = target_terminal;

    return;
}
