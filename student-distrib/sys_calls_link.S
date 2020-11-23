.globl sys_call_handler_asm


sys_call_handler_asm:


<<<<<<< HEAD
    pushf           # dont wanna mess with eax
=======
    # pushf           # dont wanna mess with eax
>>>>>>> master
    pushl %esi
    pushl %edi

    subl $1, %eax   # i need this for 0 index
    cmpl $0, %eax
    jl fail_handler
    cmpl $10, %eax   # will become 10 in cp4
    jg fail_handler

    pushl %edx
    pushl %ecx
    pushl %ebx

    pushl %edx
    pushl %ecx
    pushl %ebx
    call *jump_table(,%eax,4)
    popl %ebx
    popl %ecx
    popl %edx

    popl %ebx
    popl %ecx
    popl %edx

    popl %edi
    popl %esi
<<<<<<< HEAD
    popf
=======
    # popf
>>>>>>> master
    iret


fail_handler:
    movl $-1, %eax
    popl %edi
    popl %esi
<<<<<<< HEAD
    popf
=======
    # popf
>>>>>>> master
    iret

jump_table:
  .long sys_halt, sys_execute, sys_read, sys_write, sys_open, sys_close, sys_getargs, sys_vidmap
