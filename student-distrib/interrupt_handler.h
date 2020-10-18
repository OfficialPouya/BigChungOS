#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H

#define EXCEPTION_OBJ(name, exception_msg)
void print_exception(){
    printf("%s", #exception_msg);
    while(1);
}

void idt_setup();



#endif /* INTERRUPT_HANDLER_H */
