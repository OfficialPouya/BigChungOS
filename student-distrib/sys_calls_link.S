.globl sys_call_link

sys_call_halt:
sys_call_execute:

sys_call_handler_asm:
    cmpl $0, %eax
    jle fail_handler
    cmpl $6, %eax
    jg fail_handler
    subl $1, %eax   # i think i need this for 0 index
    jmp *jump_table(,%eax,4)

sys_call_open:
    pushl %ebx      # filename
    call sys_open
    popl %ebx       # filename
    iret

sys_call_read:
    cmpl $-1, %edx  # check for -1 (return -1 fail)
    je read_fail
    cmpl $-1, %ecx  # check for -1 (return -1 fail)
    je read_fail
    cmpl $-1, %ebx  # check for -1 (return -1 fail)
    je read_fail
    pushl %edx      # nbytes
    pushl %ecx      # buf
    pushl %ebx      # fd
    call sys_read
    popl %ebx       # fd
    popl %ecx       # buf
    popl %edx       # nbytes
    iret

sys_call_write:
    pushl %edx      # nbytes
    pushl %ecx      # buf
    pushl %ebx      # fd
    call sys_write
    popl %ebx       # fd
    popl %ecx       # buf
    popl %edx       # nbytes
    iret


sys_call_close:
    pushl %ebx      # fd
    call sys_close
    popl %ebx       # fd
    iret

read_fail:
    call sys_call_handler
    iret

fail_handler:
    movl $-1, %eax
    iret

jump_table:
  .long sys_call_halt, sys_call_execute, sys_call_read, sys_call_write, sys_call_open, sys_call_close