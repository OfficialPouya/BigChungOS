.globl sys_call_handler_asm


sys_call_handler_asm:

    
	pushf           # dont wanna mess with eax 
	pushl %ebp
    pushl %esi
	pushl %edi
	

    cmpl $0, %eax
    jle fail_handler
    cmpl $6, %eax   # will become 10 in cp4
    jg fail_handler
    subl $1, %eax   # i need this for 0 index

    pushl %ecx 
    pushl %edx
    pushl %ebx
    call *jump_table(,%eax,4)
    popl %ebx 
    popl %edx
    popl %ecx 
    
    popl %edi
    popl %esi
    popl %ebp
    popf
    iret


fail_handler:
    movl $-1, %eax
    popal
    iret

jump_table:
  .long sys_halt, sys_execute, sys_read, sys_write, sys_open, sys_close
