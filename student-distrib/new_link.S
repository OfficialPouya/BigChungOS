.globl rtc_asm
.globl keyboard_asm
.globl pit_asm


/*
 NAME: keyboard_asm
 DESCRIPTION: calls the C code for keyboard handler 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: calls C function that does CLI but also STIs later
 */
keyboard_asm:
	pushal
	call keyboard_handler
	popal
	iret


    
/*
 NAME: rtc_asm
 DESCRIPTION: calls the C code for rtc handler 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: calls C function that does CLI but also STIs later
 */
rtc_asm:
	pushal
	call handle_rtc
	popal
	iret


/*
 NAME: pit_asm
 DESCRIPTION: calls the C code that stores register values before calling our c function for the pit handler
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: NONE
 IMPACTS ON OTHERS: PIT is called everytime it ticks (20 Hz / 50ms)
 */
pit_asm:
    pushal
    call pit_handler
    popal
    iret
