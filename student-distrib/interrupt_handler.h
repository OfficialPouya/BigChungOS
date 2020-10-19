// interrupt_handler.h - Link the interrupts with C/ASM linkage
// vim:ts=4 noexpandtab

void idt_setup(void);

void rtc_C(void);
void keyboard_C(void);
void syscall_handler_C(void);
void exception0_C(void);
void exception1_C(void);
void exception2_C(void);
void exception3_C(void);
void exception4_C(void);
void exception5_C(void);
void exception6_C(void);
void exception7_C(void);
void exception8_C(void);
void exception9_C(void);
void exception10_C(void);
void exception11_C(void);
void exception12_C(void);
void exception13_C(void);
void exception14_C(void);
void exception15_C(void);
void exception16_C(void);
void exception17_C(void);
void exception18_C(void);
void exception19_C(void);
void exception20_C(void);
void exception21_C(void);
void exception22_C(void);
void exception23_C(void);
void exception24_C(void);
void exception25_C(void);
void exception26_C(void);
void exception27_C(void);
void exception28_C(void);
void exception29_C(void);
void exception30_C(void);
void exception31_C(void);

void not_specific(void);
extern void do_IRQ_temp(int first_arg);

extern void rtc_asm(void);
extern void keyboard_asm(void);
extern void syscall_handler_asm(void);
extern void exception0_asm(void);
extern void exception1_asm(void);
extern void exception2_asm(void);
extern void exception3_asm(void);
extern void exception4_asm(void);
extern void exception5_asm(void);
extern void exception6_asm(void);
extern void exception7_asm(void);
extern void exception8_asm(void);
extern void exception9_asm(void);
extern void exception10_asm(void);
extern void exception11_asm(void);
extern void exception12_asm(void);
extern void exception13_asm(void);
extern void exception14_asm(void);
extern void exception15_asm(void);
extern void exception16_asm(void);
extern void exception17_asm(void);
extern void exception18_asm(void);
extern void exception19_asm(void);
extern void exception20_asm(void);
extern void exception21_asm(void);
extern void exception22_asm(void);
extern void exception23_asm(void);
extern void exception24_asm(void);
extern void exception25_asm(void);
extern void exception26_asm(void);
extern void exception27_asm(void);
extern void exception28_asm(void);
extern void exception29_asm(void);
extern void exception30_asm(void);
extern void exception31_asm(void);
extern void rtc_asm(void);

