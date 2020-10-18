#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"

// DE, "Divide Error"
// DB, "RESERVED"
// NMI, "Non-maskable Interrupt"
// BP, "Breakpoint"
// OF, "Overflow"
// BR, "BOUND Range Exceeded"
// UD, "Invalid Opcode (undefined)"
// NM, "Device not available (No Math Coprocessor)""
// DF, "Double fault");
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

void idt_setup(){
    int i;

    for (i = 0; i < NUM_VEC; i++){
      idt[i].size = 1;              // 32-bit instruction instead of 16
      idt[i].seg_selector = KERNEL_CS;   // kernel segment
      idt[i].present = 1;           // segment is present
      idt[i].dpl = 0;
      if (i == 0x80)                // for system calls
        idt[i].dpl = 3;

      idt[i].reserved0 = 0;
      idt[i].reserved1 = 1;
      idt[i].reserved2 = 1;
      idt[i].reserved3 = 1;
      if (i < 32)
        idt[i].reserved3 = 0;

      idt[i].reserved4 = 0;


    }

// SET_IDT_ENTRY here

}
