/* rtc.c functions to handle the RTC */
#include "lib.h"
#include "rtc.h"
#include "i8259.h"

/* init_rtc
*  DESCRIPTION: Initializes the RTC by setting registers and turning on the appropriate IRQ (IRQ #8)
*  INPUTS: NONE
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: NONE
*/
void init_rtc(void){
    outb(REG_A_NMI, RTC_PORT); //select register A
    unsigned char prev = inb(CMOS_PORT); //reads current value of register B
    outb(REG_B_NMI, RTC_PORT); // select register B
    outb( prev | 0x40, CMOS_PORT); // ored with 0x40 to turn on Periodic Interrupt Enable
    enable_irq(RTC_IRQ_LINE); //enables IRQ line which is line 8 for RTC
}

/* handle_rtc
*  DESCRIPTION: handles RTC interrupts
*  INPUTS: NONE
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: NONE
*/
void handle_rtc(void){
     cli();
     outb(REG_C, RTC_PORT); // select register C
     inb(CMOS_PORT); // throw away contents
     send_eoi(RTC_IRQ_LINE);
     //test_interrupts(); //tests if RTC works properly
     sti();
     
}
