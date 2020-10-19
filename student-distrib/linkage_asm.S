# linkage_asm.S - Link the interrupts with C/ASM linkage
# vim:ts=4 noexpandtab

.text

.globl rtc_asm
.globl keyboard_asm
.globl syscall_handler_asm

.globl exception0_asm, exception1_asm, exception2_asm, exception3_asm, exception4_asm, exception5_asm, exception6_asm, exception7_asm, exception8_asm, exception9_asm, exception10_asm
.globl exception11_asm, exception12_asm, exception13_asm, exception14_asm, exception15_asm, exception16_asm, exception17_asm, exception18_asm, exception19_asm, exception20_asm
.globl exception21_asm, exception22_asm, exception23_asm, exception24_asm, exception25_asm, exception26_asm, exception27_asm, exception28_asm, exception29_asm, exception30_asm, exception31_asm

.extern do_IRQ_temp

IDT_SIZE:   .long   256				

exception0_asm:
    pushl $-256
    jmp common_interrupt

exception1_asm:
    pushl $-255
    jmp common_interrupt

exception2_asm:
    pushl $-254
    jmp common_interrupt

exception3_asm:
    pushl $-253
    jmp common_interrupt 

exception4_asm:
    pushl $-252
    jmp common_interrupt 

exception5_asm:
    pushl $-251
    jmp common_interrupt 

exception6_asm: 
    pushl $-250
    jmp common_interrupt

exception7_asm: 
    pushl $-249
    jmp common_interrupt

exception8_asm: 
    pushl $-248
    jmp common_interrupt

exception9_asm: 
    pushl $-247
    jmp common_interrupt

exception10_asm:
    pushl $-246
    jmp common_interrupt

exception11_asm: 
    pushl $-245
    jmp common_interrupt

exception12_asm: 
    pushl $-244
    jmp common_interrupt

exception13_asm: 
    pushl $-243
    jmp common_interrupt

exception14_asm: 
    pushl $-242
    jmp common_interrupt

exception15_asm: 
    pushl $-241
    jmp common_interrupt

exception16_asm: 
    pushl $-240
    jmp common_interrupt

exception17_asm: 
    pushl $-239
    jmp common_interrupt

exception18_asm: 
    pushl $-238
    jmp common_interrupt

exception19_asm: 
    pushl $-237
    jmp common_interrupt

exception20_asm:
    pushl $-236
    jmp common_interrupt

exception21_asm: 
    pushl $-235
    jmp common_interrupt

exception22_asm: 
    pushl $-234
    jmp common_interrupt

exception23_asm: 
    pushl $-233
    jmp common_interrupt

exception24_asm: 
    pushl $-232
    jmp common_interrupt

exception25_asm: 
    pushl $-231
    jmp common_interrupt

exception26_asm: 
    pushl $-230
    jmp common_interrupt

exception27_asm: 
    pushl $-229
    jmp common_interrupt

exception28_asm: 
    pushl $-228
    jmp common_interrupt

exception29_asm: 
    pushl $-227
    jmp common_interrupt

exception30_asm: 
    pushl $-226
    jmp common_interrupt

exception31_asm:
    pushl $-225
    jmp common_interrupt

syscall_handler_asm:
    pushl $-128
    jmp common_interrupt

common_interrupt:
    pushal
    movl %esp, %eax
    call do_IRQ_temp
    jmp ret_from_intr

ret_from_intr:
    popal
    IRET

/*
keyboard_asm:
    pushal
    call keyboard_handler
    popal
    iret

rtc_asm:
    pushal
    call handle_rtc
    popal
    iret
*/
