#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H


void idt_setup();

void* syscall_placeholder(char* err);

void* temp_handler();

void* temp2_handler();

#endif /* INTERRUPT_HANDLER_H */
