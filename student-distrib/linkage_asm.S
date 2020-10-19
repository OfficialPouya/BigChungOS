.globl rtc_asm
.globl keyboard_asm
.globl syscall_handler_asm
# .globl pit_asm
# .globl pit_return

keyboard_asm:
    pushal
    call keyboard_handler
    popal
    iret

syscall_handler_asm:
    pushal
    call syscall_handler_c
    popal
    iret

rtc_asm:
    pushal
    call handle_rtc
    popal
    iret
