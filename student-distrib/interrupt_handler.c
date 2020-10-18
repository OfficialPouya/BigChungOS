#include "lib.h"
#include "i8529.h"
#include "interrupt_handler.h"
#include "x86_desc.h"

EXCEPTION_OBJ(DE, "Divide Error");
EXCEPTION_OBJ(DB, "RESERVED");
EXCEPTION_OBJ(NMI, "Non-maskable Interrupt");
EXCEPTION_OBJ(BP, "Breakpoint");
EXCEPTION_OBJ(OF, "Overflow");
EXCEPTION_OBJ(BR, "BOUND Range Exceeded");
EXCEPTION_OBJ(UD, "Invalid Opcode (undefined)");
EXCEPTION_OBJ(NM, "Device not available (No Math Coprocessor)");
EXCEPTION_OBJ(DF, "Double fault");
EXCEPTION_OBJ(CSO, "Coprocessor Segment Overrun (reserved)");
EXCEPTION_OBJ(TS, "Invalid TSS");
EXCEPTION_OBJ(NP, "Segment Not Present");
EXCEPTION_OBJ(SS, "Stack-Segment Fault");
EXCEPTION_OBJ(GP, "General Protection");
EXCEPTION_OBJ(PF, "Page Fault");
// EXCEPTION_OBJ(IDK, "Intel reserved");
EXCEPTION_OBJ(MF, "x87 FPU Floating-Point Error (Math Fault)");
EXCEPTION_OBJ(AC, "Alignment Check");
EXCEPTION_OBJ(MC, "Machine Check");
EXCEPTION_OBJ(XF, "SIMD Floating-Point Error (Math Fault)");

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
    SET_IDT_ENTRY(idt[0], DE);
    SET_IDT_ENTRY(idt[1], DB);
    SET_IDT_ENTRY(idt[2], NMI);
    SET_IDT_ENTRY(idt[3], BP);
    SET_IDT_ENTRY(idt[4], OF);
    SET_IDT_ENTRY(idt[5], BR);
    SET_IDT_ENTRY(idt[6], UD);
    SET_IDT_ENTRY(idt[7], NM);
    SET_IDT_ENTRY(idt[8], DF);
    SET_IDT_ENTRY(idt[9], CSO);
    SET_IDT_ENTRY(idt[10], TS);
    SET_IDT_ENTRY(idt[11], NP);
    SET_IDT_ENTRY(idt[12], SS);
    SET_IDT_ENTRY(idt[13], GP);
    SET_IDT_ENTRY(idt[14], PF);
    // SET_IDT_ENTRY(idt[15], IDK);
    SET_IDT_ENTRY(idt[16], MF);
    SET_IDT_ENTRY(idt[17], AC);
    SET_IDT_ENTRY(idt[18], MC);
    SET_IDT_ENTRY(idt[19], XF);

    lidt(idt_desc_ptr);
}


#endif
