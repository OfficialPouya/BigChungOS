#include "x86_desc.h"
#include "keyboard.h"
#include "lib.h"
#include "interrupt_handler.h"
#include "rtc.h"

extern void keyboard_asm();
extern void handle_rtc();
extern void syscall_handler_asm();
