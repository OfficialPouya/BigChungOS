#pragma once

#include "lib.h"
#include "i8259.h"
#include "interrupt_handler.h"
#include "x86_desc.h"
#include "types.h"
#include "rtc.h"
#include "keyboard.h"
#include "sched.h"
// These are declared so we can link asm to c
extern void keyboard_asm();
extern void rtc_asm();
extern void pit_asm();
