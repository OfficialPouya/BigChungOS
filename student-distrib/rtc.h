#ifndef _RTC_H
#define _RTC_H

#include "types.h"

/* Macros to be used in RTC functions. Values and names found on osdev */ 

#define RTC_PORT  0x70 
#define CMOS_PORT 0x71

/* status registers A-D, use 0x80 to disable non-masking interrupt (NMI) */
#define REG_A  0x8A
#define REG_B  0x8B
#define REG_C  0x8C
#define REG_D  0x8D

#define RTC_IRQ_LINE 8

/* initialize the RTC */
void init_rtc(void);

/* handle interrupts */
void handle_rtc();

#endif /* _RTC_H */
