/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

// SOURCE: https://wiki.osdev.org/8259_PIC
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)



/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
//uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
     outb(master_mask, PIC1_DATA); // masking the master
     //outb(slave_mask, PIC2_DATA); // masking the slave

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint32_t mask = ~(1 << irq_num);
    master_mask = master_mask & mask;
    outb(master_mask, PIC1_DATA);
}


/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint32_t mask = (1 << irq_num);
    master_mask = master_mask | mask;
    outb(master_mask, PIC1_DATA);
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    outb(EOI | irq_num, PIC1_COMMAND);
}
