.globl rtc_asm
.globl keyboard_asm


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
