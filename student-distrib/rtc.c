/* rtc.c functions to handle the RTC */
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

// https://wiki.osdev.org/RTC

/* global variables */
int interrupt_flag = 0; //flag to know whether an interrupt has occured 
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
    interrupt_flag = 0;
    sti();     
}
/* rtc_set_frequency 
*  DESCRIPTION: helper function to be used by open_rtc and write_rtc to set the frequency of the RTC. 
*  INPUTS: frequency that the RTC should be set to 
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: sets the frequency passed in by other functions. 
*/
int32_t rtc_set_frequency(int32_t frequency){
    char rate; 
    unsigned char saved_a;
    rate = 0x0F;    // the lowest setting available (2HZ), as temp increases by 2, this decreases by 1
    int32_t temp;
    temp = 4;
    //sets rate according to maximum freauency which is 1024 Hz, rate value taken from RTC document on course webpage. 
    while (frequency > temp && temp < MAX_RATE){
        temp = temp*MIN_RATE;
        --rate;
    }
    //sets the frequency of the RTC using the selected rate
    cli();
    outb(REG_A_NMI, RTC_PORT); // select register A
    saved_a = inb(CMOS_PORT); // save the previous value of A 
    outb(REG_A_NMI, RTC_PORT); // select register A
    outb( (saved_a & 0xF0) | rate , CMOS_PORT); // saves the upper 4 bits of register A and sets low 4 bits of register A with the selected rate
    sti();
    return 4; 
}
/* rtc_open
*  DESCRIPTION: Opens the RTC and sets the RTC frequency to 2 and returns 0. 
*  INPUTS: pointer to file
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: sets the frequency to 2
*/
int32_t rtc_open(const uint8_t* filename){
    //sets the frequency for virtualization
    rtc_set_frequency(MIN_RATE);
    return 0; 
}
/* rtc_read
*  DESCRIPTION: Takes care of virtualization process by waiting until there have been x interrupts (1024/freq) and blocks before next interrupt
*  before returning 0. 
*  INPUTS: file descriptor, buffer pointer, number of bytes passed in 
*  OUTPUTS: returns 0 
*  IMPACTS ON OTHERS: NONE
*/
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    //wait until interrupt has occurred 
    interrupt_flag = 1;
    sti();
    while(interrupt_flag){
    }
    //once interrupt has occurred, reset counter and return 0 
    return 0;
}
/* rtc_write
*  DESCRIPTION: Gets the frequency from the buffer
*  INPUTS: file descriptor, buffer pointer, number of bytes passed in 
*  OUTPUTS: -1 on fail, nbytes on pass
*  IMPACTS ON OTHERS: sets the frequency global variable to be used in rtc_read. 
*/
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes){ 
    int32_t freq;
    //rtc_write must read 4 bytes, and must take in a frequency from the buffer. 
    if (nbytes != 4 || buf == NULL){
        return -1; 
    }
    freq = *((int32_t*) buf); // get frequency from buffer
    if(((freq&(freq - 1)) || freq > MAX_RATE) != 0){  //checks if frequency is a power of 2 and within bounds of maximum frequency
        return -1;
    }

    return rtc_set_frequency(freq); 
}

/* rtc_close
*  DESCRIPTION: Closes the RTC. 
*  INPUTS: file descriptor
*  OUTPUTS: returns 0 
*  IMPACTS ON OTHERS: NONE
*/
int32_t rtc_close(int32_t fd){
    rtc_set_frequency(MIN_RATE);
    return 0;
}
