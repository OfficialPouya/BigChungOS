#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"
#include "rtc.h"
#include "keyboard.h"
#include "linkage_asm.h"
#include "types.h"


#define SYSCALL_NUM 128 // 0x80
#define KEYBOARD_ADDR 33
#define RTC_ADDR 40
#define NUM_IDT_ENTRIES 47 // number of entries in IDT that are defined
#define KILLED_BY_EXCEPTION 256 // found on OSDEV

// DE, "Divide Error"
// DB, "RESERVED"
// NMI, "Non-maskable Interrupt"
// BP, "Breakpoint"
// OF, "Overflow"
// BR, "BOUND Range Exceeded"
// UD, "Invalid Opcode (undefined)"
// NM, "Device not available (No Math Coprocessor)"
// DF, "Double fault"
// CSO, "Coprocessor Segment Overrun (reserved)"
// TS, "Invalid TSS"
// NP, "Segment Not Present"
// SS, "Stack-Segment Fault"
// GP, "General Protection"
// PF, "Page Fault"
// IDK, "Intel reserved"
// MF, "x87 FPU Floating-Point Error (Math Fault)"
// AC, "Alignment Check"
// MC, "Machine Check"
// XF, "SIMD Floating-Point Error (Math Fault)"



void de() {
    clear();
    printf("ERROR 1\n");
    while(1);
} //divide error
void db() {
    clear();
    printf("ERROR 2\n");
    while(1);
} //reserved  (reserved)
void nmi() {
    clear();
    printf("ERROR 3\n");
    while(1);
} //nonmaskable external interrupt
void bp() {
    clear();
    printf("ERROR 4\n");
    while(1);
} //breakpoint
void of() {
    clear();
    printf("ERROR 5\n");
    while(1);
} //overflow
void br() {
    clear();
    printf("ERROR 6\n");
    while(1);
} //bound range exceeded
void ud() {
    clear();
    printf("ERROR 7\n");
    while(1);
} //invalid opcode
void nm() {
    clear();
    printf("ERROR 8\n");
    while(1);
} //device not available
void df() {
    clear();
    printf("ERROR 9\n");
    while(1);
} //double fault --> return zero always
void cpso(){
    clear();
    printf("ERROR 10\n");
    while(1);
} //coprocessor segment overrun (reserved)
void ts() {
    clear();
    printf("ERROR 11\n");
    while(1);
} //invalid tss
void np() {
    clear();
    printf("ERROR 12\n");
    while(1);
} //segment not present
void ssf() {
    clear();
    printf("ERROR 13\n");
    while(1);
} //stack segment fault
void gp() {
    clear();
    printf("ERROR 14\n");
    while(1);
} //general protection
void pf() {
    clear();
    printf("Page Fault\n");
    while(1);
} //page fault
// void ir() {
//     printf("ERROR 16\n");
//     while(1);
// } //intel reserved do not use
void mf() {
    clear();
    printf("ERROR 17\n");
    while(1);
} //x87 FPU error
void ac() {
    clear();
    printf("ERROR 18\n");
    while(1);
} //alignment check ---> return zero
void mc() {
    clear();
    printf("ERROR 19\n");
    while(1);
} //machine check
void xf() {
    clear();
    printf("ERROR 20\n");
    while(1);
} //simd floating point exception
void gen_purp() {
    clear();
    printf("ERROR NOT YET DEFINED");
    while(1);
}
void syscall_handler_c(){
  printf("SYSCALL ATTEMPTED");
  while(1);
}

void idt_setup(){
    int i;

    for (i = 0; i < NUM_IDT_ENTRIES; i++){
      idt[i].size = 1;              // 32-bit instruction instead of 16
      idt[i].seg_selector = KERNEL_CS;   // kernel segment
      idt[i].present = 1;           // segment is present
      idt[i].dpl = 0;


      idt[i].reserved0 = 0;
      idt[i].reserved1 = 1;
      idt[i].reserved2 = 1;
      idt[i].reserved3 = 0;
      if (i == SYSCALL_NUM){
        // idt[i].reserved3 = 1;
        idt[i].dpl = 3;
      }

      idt[i].reserved4 = 0;
    }
    SET_IDT_ENTRY(idt[0], de);
    SET_IDT_ENTRY(idt[1], db);
    SET_IDT_ENTRY(idt[2], nmi);
    SET_IDT_ENTRY(idt[3], bp);
    SET_IDT_ENTRY(idt[4], of);
    SET_IDT_ENTRY(idt[5], br);
    SET_IDT_ENTRY(idt[6], ud);
    SET_IDT_ENTRY(idt[7], nm);
    SET_IDT_ENTRY(idt[8], df);
    SET_IDT_ENTRY(idt[9], cpso);
    SET_IDT_ENTRY(idt[10], ts);
    SET_IDT_ENTRY(idt[11], np);
    SET_IDT_ENTRY(idt[12], ssf);
    SET_IDT_ENTRY(idt[13], gp);
    SET_IDT_ENTRY(idt[14], pf);
    // SET_IDT_ENTRY(idt[15], ir);
    SET_IDT_ENTRY(idt[16], mf);
    SET_IDT_ENTRY(idt[17], ac);
    SET_IDT_ENTRY(idt[18], mc);
    SET_IDT_ENTRY(idt[19], xf);
    SET_IDT_ENTRY(idt[KEYBOARD_ADDR], keyboard_asm);
    SET_IDT_ENTRY(idt[SYSCALL_NUM], syscall_handler_asm);
    SET_IDT_ENTRY(idt[RTC_ADDR], handle_rtc);
    // SET_IDT_ENTRY here
    // lidt(idt_desc_ptr);

}
