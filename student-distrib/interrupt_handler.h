#ifndef INTERRUPT_HANDLER_H
#define INTERRUPT_HANDLER_H


void idt_setup();
void de();
void db();
void nmi();
void bp();
void of();
void br();
void ud();
void nm();
void df();
void cpso();
void ts();
void np();
void ssf();
void gp();
void pf();
// void ir();
void mf();
void ac();
void mc();
void xf();
void gen_purp();
void syscall_handler_c();

#endif /* INTERRUPT_HANDLER_H */
