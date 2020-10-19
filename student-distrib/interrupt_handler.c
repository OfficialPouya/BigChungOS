#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"
#include "rtc.h"
#include "keyboard.h"


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

void* syscall_placeholder(char* err){
  printf("%s", err);
  while(1);

}

void* temp_handler(){
  printf("temp_handler");
  while(1);
}

void* temp2_handler(){
  printf("temp2_handler");
  while(1);
}

void idt_setup(){
    int i;

    for (i = 0; i < NUM_VEC; i++){
      idt[i].size = 1;              // 32-bit instruction instead of 16
      idt[i].seg_selector = KERNEL_CS;   // kernel segment
      idt[i].present = 1;           // segment is present
      idt[i].dpl = 0;
      

      idt[i].reserved0 = 0;
      idt[i].reserved1 = 1;
      idt[i].reserved2 = 1;
      idt[i].reserved3 = 0;
      if (i == 0x80){
        // idt[i].reserved3 = 1;
        idt[i].dpl = 3;
      }

      idt[i].reserved4 = 0;
    }
    SET_IDT_ENTRY(idt[0], temp_handler);
    SET_IDT_ENTRY(idt[1], temp2_handler);
    SET_IDT_ENTRY(idt[2], temp_handler);
    SET_IDT_ENTRY(idt[3], temp_handler);
    SET_IDT_ENTRY(idt[4], temp_handler);
    SET_IDT_ENTRY(idt[5], temp_handler);
    SET_IDT_ENTRY(idt[6], temp_handler);
    SET_IDT_ENTRY(idt[7], temp_handler);
    SET_IDT_ENTRY(idt[8], temp_handler);
    SET_IDT_ENTRY(idt[9], temp_handler);
    SET_IDT_ENTRY(idt[10], temp_handler);
    SET_IDT_ENTRY(idt[11], temp_handler);
    SET_IDT_ENTRY(idt[12], temp_handler);
    SET_IDT_ENTRY(idt[13], temp_handler);
    SET_IDT_ENTRY(idt[14], temp_handler);

    SET_IDT_ENTRY(idt[16], temp_handler);
    SET_IDT_ENTRY(idt[17], temp_handler);
    SET_IDT_ENTRY(idt[18], temp_handler);
    SET_IDT_ENTRY(idt[19], temp_handler);




    SET_IDT_ENTRY(idt[0x21], keyboard_handler);
    SET_IDT_ENTRY(idt[0x28], handle_rtc);
// SET_IDT_ENTRY here
    lidt(idt_desc_ptr);

}
