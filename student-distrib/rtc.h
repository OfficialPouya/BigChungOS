#ifndef _RTC_H
#define _RTC_H
#pragma once
#include "types.h"

/* Macros to be used in RTC functions. Values and names found on osdev */ 

#define MAX_RATE 1024
#define MIN_RATE 2

#define RTC_PORT  0x70 
#define CMOS_PORT 0x71

/* status registers A-D, use 0x80 to disable non-masking interrupt (NMI) */
#define REG_A_NMI  0x8A
#define REG_B_NMI  0x8B
#define REG_C_NMI  0x8C
#define REG_D_NMI  0x8D

#define REG_A  0x0A
#define REG_B  0x0B
#define REG_C  0x0C
#define REG_D  0x0D

#define RTC_IRQ_LINE 8

/* initialize the RTC */
void init_rtc(void);

/* handle interrupts */
void handle_rtc(void);

/* sets frequency */
int32_t rtc_set_frequency(int32_t frequency);

/* opens the RTC and sets frequency to 2 */ 
int32_t rtc_open(const uint8_t* filename);

/* blocks until next interrupt, also handles virtualization */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

/* gets frequency from buffer */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);

/* closes the RTC */
int32_t rtc_close(int32_t fd);


#endif /* _RTC_H */
