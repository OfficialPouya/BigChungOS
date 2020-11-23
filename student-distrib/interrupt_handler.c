#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"
#include "types.h"
#include "rtc.h"
#include "keyboard.h"
#include "new_link.h"
#include "sys_calls_link.h"
#include "sys_calls.h"
<<<<<<< HEAD
=======
#include "sched.h"
>>>>>>> master
#define SYSCALL_NUM 128 // 0x80
#define KEYBOARD_ADDR 0x21 // addr for kb 
#define RTC_ADDR 0x28 // addr for rtc 
#define NUM_IDT_ENTRIES 256 // we need to fill IDT all the way no matter what 
<<<<<<< HEAD
#define HALT_BY_EXCEPTION 256
=======
#define HALT_BY_EXCEPTION 256 // defined in docs
#define PIT_ADDR 0x20
>>>>>>> master

/*
 NAME: exception0_C - exception19_C
 DESCRIPTION: function is called when exception occurs, and
                holds function in while(1) loop and prints the
                error that occured
 INPUTS: NONE
 OUTPUTS: Prints error message
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: will stay in loop, have to terminate the program 
 */

void exception0_C(void) {
    clear();
    printf("Divide-by-zero Error\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //divide error

void exception1_C(void) {
    clear();
    printf("Debug\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //reserved  (reserved)

void exception2_C(void) {
    clear();
    printf("Non-maskable Interrupt\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //nonmaskable external interrupt

void exception3_C(void) {
    clear();
    printf("Breakpoint\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //breakpoint

void exception4_C(void) {
    clear();
    printf("Overflow\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //overflow

void exception5_C(void) {
    clear();
    printf("Bound Range Exceeded\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //bound range exceeded

void exception6_C(void) {
    clear();
    printf("Invalid Opcode\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //invalid opcode

void exception7_C(void) {
    clear();
    printf("Device Not Available\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //device not available

void exception8_C(void) {
    clear();
    printf("Double Fault\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //double fault --> return zero always

void exception9_C(void){
    clear();
    printf("Coprocessor Segment Overrun\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //coprocessor segment overrun (reserved)

void exception10_C(void) {
    clear();
    printf("Invalid TSS\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //invalid tss

void exception11_C(void) {
    clear();
    printf("Segment Not Present\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //segment not present

void exception12_C(void) {
    clear();
    printf("Stack-Segment Fault\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //stack segment fault

void exception13_C(void) {
    clear();
    printf("General Protection Fault\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //general protection

void exception14_C(void) {
    clear();
    printf("Page Fault\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //page fault

void exception15_C(void) {
    printf("Reserved\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //intel reserved do not use

void exception16_C(void) {
    clear();
    printf("x87 FPU Floating-Point Error\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //x87 FPU error

void exception17_C(void) {
    clear();
    printf("Alignment Check\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //alignment check ---> return zero

void exception18_C(void) {
    clear();
    printf("Machine Check\n");
<<<<<<< HEAD
=======
    flag_exception = 1;
>>>>>>> master
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //machine check

void exception19_C(void) {
    clear();
    printf("SIMD Floating-Point Error\n");
<<<<<<< HEAD
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //simd floating point exception

=======
    flag_exception = 1;
    sys_halt((uint8_t) HALT_BY_EXCEPTION);
} //simd floating point exception


>>>>>>> master
// void sys_call_handler_C(void) {
//     clear();
//     printf("Sys Call Happened\n");
//     while(1);
// } //intel reserved do not use



/*
 NAME: IDT SETUP
 DESCRIPTION: this sets the interrupt gate
 INPUTS: value of idt 
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: fills out the IDT tables gates to interrupt
 */
//https://wiki.osdev.org/Interrupt_Descriptor_Table#I386_Interrupt_Gate
void idt_setup(int i){
    idt[i].seg_selector = KERNEL_CS;
    idt[i].dpl = 0;
    idt[i].reserved0 = 0;
    idt[i].reserved1 = 1;
    idt[i].reserved2 = 1;
    idt[i].reserved3 = 0;
    idt[i].reserved4 = 0;
    idt[i].present = 1;
    idt[i].size = 1;

    if (i == SYSCALL_NUM) {
        idt[i].dpl = 3;
<<<<<<< HEAD
         idt[i].reserved0 = 0;
=======
        idt[i].reserved0 = 0;
>>>>>>> master
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;
        idt[i].reserved4 = 0;
    }
}



/*
 NAME: idt_vector
 DESCRIPTION: inits the defined IDT items
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: inits and sets idt table
 */
void idt_vector(){
    int i;
    for (i = 0; i < NUM_IDT_ENTRIES; i++) {
        idt_setup(i);
    }
    
    SET_IDT_ENTRY(idt[0], exception0_C);
    SET_IDT_ENTRY(idt[1], exception1_C);
    SET_IDT_ENTRY(idt[2], exception2_C);
    SET_IDT_ENTRY(idt[3], exception3_C);
    SET_IDT_ENTRY(idt[4], exception4_C);
    SET_IDT_ENTRY(idt[5], exception5_C);
    SET_IDT_ENTRY(idt[6], exception6_C);
    SET_IDT_ENTRY(idt[7], exception7_C);
    SET_IDT_ENTRY(idt[8], exception8_C);
    SET_IDT_ENTRY(idt[9], exception9_C);
    SET_IDT_ENTRY(idt[10], exception10_C);
    SET_IDT_ENTRY(idt[11], exception11_C);
    SET_IDT_ENTRY(idt[12], exception12_C);
    SET_IDT_ENTRY(idt[13], exception13_C);
    SET_IDT_ENTRY(idt[14], exception14_C);
    SET_IDT_ENTRY(idt[15], exception15_C);
    SET_IDT_ENTRY(idt[16], exception16_C);
    SET_IDT_ENTRY(idt[17], exception17_C);
    SET_IDT_ENTRY(idt[18], exception18_C);
    SET_IDT_ENTRY(idt[19], exception19_C);
    SET_IDT_ENTRY(idt[RTC_ADDR], rtc_asm);
    SET_IDT_ENTRY(idt[PIT_ADDR], pit_asm);
    SET_IDT_ENTRY(idt[KEYBOARD_ADDR], keyboard_asm);
    SET_IDT_ENTRY(idt[SYSCALL_NUM], sys_call_handler_asm);
    
}
