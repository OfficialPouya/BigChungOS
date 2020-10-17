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
    outb(REG_A, RTC_PORT); //select register A
    unsigned char prev = inb(CMOS_PORT); //reads current value of register B
    outb(REG_B, RTC_PORT); // select register B
    outb( prev | 0x40, CMOS_PORT); // ored with 0x40 to turn on Periodic Interrupt Enable
    enable_irq(RTC_IRQ_LINE); //enables IRQ line which is line 8 for RTC
}

void handle_rtc(){

}
