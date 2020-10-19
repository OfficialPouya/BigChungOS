/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

// SOURCE: https://wiki.osdev.org/8259_PIC
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1) // data port is always 1 after command 
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1) // dara port is always 1 after command 

// Control words to init each PIC.
#define ICW1                0x11
#define ICW2_MASTER         0x20
#define ICW2_SLAVE          0x28
#define ICW3_MASTER         0x04
#define ICW3_SLAVE          0x02
#define ICW4                0x01



/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFB; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {
    // masking the master
    outb(master_mask, PIC1_DATA);
    // masking the slave
    outb(slave_mask, PIC2_DATA);

    // set out the command words for master 
    outb(ICW1, PIC1_COMMAND);
    outb(ICW2_MASTER, PIC1_DATA);
    outb(ICW3_MASTER, PIC1_DATA);
    outb(ICW4, PIC1_DATA);

    // set out the command words for slave
    outb(ICW1, PIC2_COMMAND);
    outb(ICW2_SLAVE, PIC2_DATA);
    outb(ICW3_SLAVE, PIC2_DATA);
    outb(ICW4, PIC2_DATA);

    // mask the slave
    outb(master_mask, PIC1_DATA);
    // mask the slave
    outb(slave_mask, PIC2_DATA);
}


/*
 NAME: enable_irq
 DESCRIPTION: Enable (unmask) the specified IRQ 
 INPUTS: IRQ Number
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: Does unmask passed IRQ
 */
void enable_irq(uint32_t irq_num) {
    // invalid IRQ NUM check IRQ RANGE is 0-15
    while(irq_num>15 || irq_num<0){return;} 

    uint32_t mask = ~(1 << irq_num);
    // here the 8 signifies the size of the PIC
    if (irq_num >= 8) {
        uint32_t unmask = ~(1 << (irq_num - 8));
        slave_mask = slave_mask & unmask;
        outb(slave_mask, PIC2_DATA);
    } 
    else {
        master_mask =  master_mask & mask;
        outb(master_mask, PIC1_DATA);
    }
}

/*
 NAME: disable_irq
 DESCRIPTION: Disable (mask) the specified IRQ
 INPUTS: IRQ Number
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: Does mask passed IRQ
 */
void disable_irq(uint32_t irq_num) {
    // invalid IRQ NUM check IRQ RANGE is 0-15
    while(irq_num>15 || irq_num<0){return;} 
    uint32_t mask = (1 << irq_num);
    master_mask = master_mask | mask;
    slave_mask = slave_mask | mask;

    // this is selection of SLAVE or MASTER
    if (irq_num & 8) {
        outb(slave_mask, PIC2_DATA);
    } 
    else {
        outb(master_mask, PIC1_DATA);
    }
}



/*
 NAME: send_eoi
 DESCRIPTION: Send end-of-interrupt signal for the specified IRQ 
 INPUTS: IRQ Number
 OUTPUTS: None
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: does cause EOI, which means system can get more interrupts
 */
void send_eoi(uint32_t irq_num) {
    // so if its bigger than 8, we know its SLAVE
    // then we need to OR with 2 so Master knows Slave 
    // got the EOI
    if (irq_num >= 8) {
        outb(EOI | (irq_num - 8), PIC2_COMMAND);
        outb(EOI | 2, PIC1_COMMAND);
    } 
    else{outb(EOI | irq_num, PIC1_COMMAND);}
        
}
