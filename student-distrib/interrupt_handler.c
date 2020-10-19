#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"
#include "types.h"

#define SYSCALL_NUM 128 // 0x80
#define KEYBOARD_ADDR 33
#define RTC_ADDR 40
#define NUM_IDT_ENTRIES 32 // number of entries in IDT that are defined

void exception0_C(void) {
    clear();
    printf("Divide-by-zero Error\n");
    while(1);
} //divide error

void exception1_C(void) {
    clear();
    printf("Debug\n");
    while(1);
} //reserved  (reserved)

void exception2_C(void) {
    clear();
    printf("Non-maskable Interrupt\n");
    while(1);
} //nonmaskable external interrupt

void exception3_C(void) {
    clear();
    printf("Breakpoint\n");
    while(1);
} //breakpoint

void exception4_C(void) {
    clear();
    printf("Overflow\n");
    while(1);
} //overflow

void exception5_C(void) {
    clear();
    printf("Bound Range Exceeded\n");
    while(1);
} //bound range exceeded

void exception6_C(void) {
    clear();
    printf("Invalid Opcode\n");
    while(1);
} //invalid opcode

void exception7_C(void) {
    clear();
    printf("Device Not Available\n");
    while(1);
} //device not available

void exception8_C(void) {
    clear();
    printf("Double Fault\n");
    while(1);
} //double fault --> return zero always

void exception9_C(void){
    clear();
    printf("Coprocessor Segment Overrun\n");
    while(1);
} //coprocessor segment overrun (reserved)

void exception10_C(void) {
    clear();
    printf("Invalid TSS\n");
    while(1);
} //invalid tss

void exception11_C(void) {
    clear();
    printf("Segment Not Present\n");
    while(1);
} //segment not present

void exception12_C(void) {
    // clear();
    printf("Stack-Segment Fault\n");
    while(1);
} //stack segment fault

void exception13_C(void) {
    clear();
    printf("General Protection Fault\n");
    while(1);
} //general protection

void exception14_C(void) {
    clear();
    printf("Page Fault\n");
    while(1);
} //page fault

void exception15_C(void) {
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception16_C(void) {
    clear();
    printf("x87 FPU Floating-Point Error\n");
    while(1);
} //x87 FPU error

void exception17_C(void) {
    clear();
    printf("Alignment Check\n");
    while(1);
} //alignment check ---> return zero

void exception18_C(void) {
    clear();
    printf("Machine Check\n");
    while(1);
} //machine check

void exception19_C(void) {
    clear();
    printf("SIMD Floating-Point Error\n");
    while(1);
} //simd floating point exception

void exception20_C(void) {
    clear();
    printf("Virtualization Exception");
    while(1);
}

void exception21_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception22_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception23_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception24_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception25_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception26_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception27_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception28_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception29_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void exception30_C(void) {
    clear();
    printf("Security Exception\n");
    while(1);
}    

void exception31_C(void) {
    clear();
    printf("Reserved\n");
    while(1);
} //intel reserved do not use

void sys_call_handler_C(void) {
    clear();
    printf("Sys Call Happened\n");
    while(1);
} //intel reserved do not use

void not_specific(void) {
    clear();
    printf("You broke it :(");
    while(1);
}

void do_IRQ_temp(int first_arg){
    uint32_t gimme_eax;
    asm("movl %%eax, %0;" : "=r" (gimme_eax) : );
    
    cli();
        switch(gimme_eax+NUM_VEC){
            case 0 :
                exception0_C();
                break;
            case 1 :
                exception1_C();
                break;
            case 2 :
                exception2_C();
                break;
            case 3 :
                exception3_C();
                break;
            case 4 :
                exception4_C();
                break;
            case 5 :
                exception5_C();
                break;
            case 6 :
                exception6_C();
                break;
            case 7 :
                exception7_C();
                break;
            case 8 :
                exception8_C();
                break;
            case 9 :
                exception9_C();
                break;
            case 10 :
                exception10_C();
                break;
            case 11 :
                exception11_C();
                break;
            case 12 :
                exception12_C();
                break;
            case 13 :
                exception13_C();
                break;
            case 14 :
                exception14_C();
                break;
            case 15 :
                exception15_C();
                break;
            case 16 :
                exception16_C();
                break;
            case 17 :
                exception17_C();
                break;
            case 18 :
                exception18_C();
                break;
            case 19 :
                exception19_C();
                break;
            case 20 :
                exception20_C();
                break;
            case 21 :
                exception21_C();
                break;
            case 22 :
                exception22_C();
                break;
            case 23 :
                exception23_C();
                break;
            case 24 :
                exception24_C();
                break;
            case 25 :
                exception25_C();
                break;
            case 26 :
                exception26_C();
                break;
            case 27 :
                exception27_C();
                break;
            case 28 :
                exception28_C();
                break;
            case 29 :
                exception29_C();
                break;
            case 30 :
                exception30_C();
                break;
            case 31 :
                exception31_C();
                break;
            case 126 :
                sys_call_handler_C();
                break;
            default :
                not_specific();    
        }
    sti();
    return;
}

void idt_setup(void){
    
    int i;
    for (i = 0; i < NUM_IDT_ENTRIES; i++){               
        // idt[i].seg_selector = KERNEL_CS;          
        // idt[i].reserved0 = 0;
        // idt[i].reserved1 = 1;
        // idt[i].reserved2 = 1;
        // idt[i].reserved3 = 0;
        // idt[i].reserved4 = 0;
        // idt[i].size = 1;    // 32-bit instruction instead of 16
        // idt[i].dpl = 0;     // kernel segment 
        // idt[i].present = 1; // segment is present 
        
        if(i==1){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  0;
            idt[i].size =       1; 
            idt[i].dpl =        0;
            idt[i].present =    1;    
        }

        if(i==2 || i==14){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  1;
            idt[i].size =       1; 
            idt[i].dpl =        0;
            idt[i].present =    1;       
        }

        if(i==3){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  1;
            idt[i].size =       1; 
            idt[i].dpl =        3;
            idt[i].present =    1;    
        }

        if(i==4 || i==5){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  0;
            idt[i].size =       1; 
            idt[i].dpl =        3;
            idt[i].present =    1;  
        }

        if(i>=6 && i <=13){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  0;
            idt[i].size =       1; 
            idt[i].dpl =        0;
            idt[i].present =    1;             
        }

        if(i>=15 && i <=31){
            idt[i].seg_selector = KERNEL_CS; 
            idt[i].reserved0 =  0;
            idt[i].reserved1 =  1;
            idt[i].reserved2 =  1;
            idt[i].reserved3 =  0;
            idt[i].size =       1; 
            idt[i].dpl =        0;
            idt[i].present =    1;             
        }
    }

    SET_IDT_ENTRY(idt[0], exception0_asm);
    SET_IDT_ENTRY(idt[1], exception1_asm);
    SET_IDT_ENTRY(idt[2], exception2_asm);
    SET_IDT_ENTRY(idt[3], exception3_asm);
    SET_IDT_ENTRY(idt[4], exception4_asm);
    SET_IDT_ENTRY(idt[5], exception5_asm);
    SET_IDT_ENTRY(idt[6], exception6_asm);
    SET_IDT_ENTRY(idt[7], exception7_asm);
    SET_IDT_ENTRY(idt[8], exception8_asm);
    SET_IDT_ENTRY(idt[9], exception9_asm);
    SET_IDT_ENTRY(idt[10], exception10_asm);
    SET_IDT_ENTRY(idt[11], exception11_asm);
    SET_IDT_ENTRY(idt[12], exception12_asm);
    SET_IDT_ENTRY(idt[13], exception13_asm);
    SET_IDT_ENTRY(idt[14], exception14_asm);
    SET_IDT_ENTRY(idt[15], exception15_asm);
    SET_IDT_ENTRY(idt[16], exception16_asm);
    SET_IDT_ENTRY(idt[17], exception17_asm);
    SET_IDT_ENTRY(idt[18], exception18_asm);
    SET_IDT_ENTRY(idt[19], exception19_asm);
    SET_IDT_ENTRY(idt[20], exception20_asm);
    SET_IDT_ENTRY(idt[21], exception21_asm);
    SET_IDT_ENTRY(idt[22], exception22_asm);
    SET_IDT_ENTRY(idt[23], exception23_asm);
    SET_IDT_ENTRY(idt[24], exception24_asm);
    SET_IDT_ENTRY(idt[25], exception25_asm);
    SET_IDT_ENTRY(idt[26], exception26_asm);
    SET_IDT_ENTRY(idt[27], exception27_asm);
    SET_IDT_ENTRY(idt[28], exception28_asm);
    SET_IDT_ENTRY(idt[29], exception29_asm);
    SET_IDT_ENTRY(idt[30], exception30_asm);
    SET_IDT_ENTRY(idt[31], exception31_asm);

    SET_IDT_ENTRY(idt[SYSCALL_NUM], syscall_handler_asm);
    idt[SYSCALL_NUM].seg_selector = KERNEL_CS; 
    idt[SYSCALL_NUM].reserved0 =  0;
    idt[SYSCALL_NUM].reserved1 =  1;
    idt[SYSCALL_NUM].reserved2 =  1;
    idt[SYSCALL_NUM].reserved3 =  0;
    idt[SYSCALL_NUM].size =       1; 
    idt[SYSCALL_NUM].dpl =        0;
    idt[SYSCALL_NUM].present =    1;    
    
    //SET_IDT_ENTRY(idt[KEYBOARD_ADDR], keyboard_asm);
    //SET_IDT_ENTRY(idt[RTC_ADDR], handle_rtc);
}
