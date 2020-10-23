/* rtc.c functions to handle the RTC */
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

/* global variables */
int interrupt_flag = 0; //flag to know whether an interrupt has occured 
int32_t freq=0; 
int32_t counter=1; 
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
     counter++;
     //test_interrupts(); //tests if RTC works properly
     sti();     
}
/* rtc_set_frequency 
*  DESCRIPTION: helper function to be used by open_rtc and write_rtc to set the frequency of the RTC. 
*  INPUTS: frequency that the RTC should be set to 
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: sets the frequency passed in by other functions. 
*/
void rtc_set_frequency(int32_t frequency){
    char rate; 
    unsigned char saved_a; 
    //frequency should not exceed 1024
    if(frequency > 1024){
        frequency = 1024; 
    }
    //sets rate according to frequency, rate values taken from RTC document. 
    if(frequency == 2){
        rate = 0x0F;
    }
    if(frequency == 4){
        rate = 0x0E;
    }
    if(frequency == 8){
        rate = 0x0D;
    }
    if(frequency == 16){
        rate = 0x0C;
    }
    if(frequency == 32){
        rate = 0x0B;
    }
    if(frequency == 64){
        rate = 0x0A; 
    }
    if(frequency == 128){
        rate = 0x09;
    }
    if(frequency == 256){
        rate = 0x08;
    }
    if(frequency == 512){
        rate = 0x07;
    }
    if(frequency == 1024){
        rate = 0x06;
    }
    //sets the frequency of the RTC using the selected rate
    cli();
    outb(REG_A_NMI, RTC_PORT); // select register A
    saved_a = inb(CMOS_PORT); // save the previous value of A 
    outb(REG_A_NMI, RTC_PORT); // select register A
    outb( (saved_a & 0xF0) | rate , CMOS_PORT); // saves the upper 4 bits of register A and sets low 4 bits of register A with the selected rate
    sti();
}
/* rtc_open
*  DESCRIPTION: Opens the RTC and sets the RTC frequency to 2 and returns 0. 
*  INPUTS: pointer to file
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: sets the frequency to 2
*/
int32_t rtc_open(const uint8_t* filename){
    int32_t temp = 2;
    rtc_set_frequency(temp); // initialize frequency to 2 on open
    return 0; 
}

int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    //wait until interrupt has occurred 
    int32_t x = 1024/freq; 
    while(counter != x ){
    }
    //once interrupt has occurred, reset flag and return 0
    counter = 1;
    return 0;
}

int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes){ 
    //rtc_write must read 4 bytes, and must take in a frequency from the buffer. 
    if (nbytes != 4 || buf == NULL){
        return -1; 
    }
    freq = *((int32_t*) buf); // get frequency from buffer
    if((freq&(freq - 1)) != 0){  //checks if frequency is a power of 2
        return -1;
    }
    //sets the frequency
    else{
        rtc_set_frequency(1024);
    }
    return nbytes; 
}

/* rtc_close
*  DESCRIPTION: Closes the RTC. 
*  INPUTS: file descriptor
*  OUTPUTS: NONE
*  IMPACTS ON OTHERS: NONE
*/
int32_t rtc_close(int32_t fd){
    return 0;
}
