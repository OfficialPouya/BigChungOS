#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "keyboard.h"
#include "terminal.h"
#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}


/*
 NAME: div_z
 DESCRIPTION: tries to divide by 0 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: Should never reach return 0 
 IMPACTS ON OTHERS: should cause exception
 */
int div_z(){
	int x = 0;
	int y= 4;
	y = y/x;
	return 0;
}


/* Paging Test NULL
*
* Tries to dereference null
*
* Inputs: None
* Outputs: PASS/FAIL
* Side Effects: None
* Coverage: init paging
* Files: memory.c/.h 
*/
int page_test_null() {
	TEST_HEADER;

	int result = PASS;
	uint32_t *temp = NULL;
	*temp = 3;

	return result;
}

/* Paging Test Outside boundaries
*
* Tries to reference outside memory space, 0xFFFFFFFF
*
* Inputs: None
* Outputs: PASS/FAIL
* Side Effects: None
* Coverage: init paging
* Files: memory.c/.h 
*/
int page_test_deref_out() {
	TEST_HEADER;

	int result = PASS;
	uint32_t *temp = (uint32_t *) 0xFFFFFFF;
	*temp = 3;

	return result;
}

// add more tests here

/* Checkpoint 2 tests */
int read_test(){
	TEST_HEADER;
	int tst;
	int results = PASS;
	char test_buffer[16];
	test_buffer[0] = 'A';
	test_buffer[1] = 'a';
	test_buffer[2] = 'm';
	test_buffer[3] = 'i';
	test_buffer[4] = 'r';
	test_buffer[5] = ' ';
	test_buffer[6] = 'i';
	test_buffer[7] = 's';
	test_buffer[8] = ' ';
	test_buffer[9] = 'C';
	test_buffer[10] = '0';
	test_buffer[11] = '0';
	test_buffer[12] = 'L';
	test_buffer[13] = '!';
	test_buffer[14] = '!';
	test_buffer[15] = '\n';
	printf("\nPRESS ENTER TO RUN");
	// number 16 is size of the buffer here
	// number 2 is the fd number 
	memmove(keyboard_buffer, &test_buffer, 16);
    tst = terminal_read(2, keyboard_buffer, 16);

    if (tst != 15) {results = FAIL;} 
	else {results = PASS;}

    memset(&keyboard_buffer, 0, 128);

    return results;
}


int write_test() {
    TEST_HEADER;

    int result = PASS;
    int temp;
    char test_buffer[3];
    test_buffer[0] = '3';
   	test_buffer[1] = '9';
    test_buffer[2] = '1';

    // number 3 is the size of the buffer here
	// number 2 is the fd number 
    temp = terminal_write(2, test_buffer, 3);

    if (temp != 3) {result = FAIL;}
	else {result = PASS;}

    return result;
}


int read_n_write_test() {
    TEST_HEADER;
	int tst, temp;
    int result = FAIL;
	char test_buffer[128];
	while(1){
		printf("Type something: ");
		tst = terminal_read(2, keyboard_buffer, 128);
		printf("\nYou typed: ");
		temp = terminal_write(2, keyboard_buffer, 128);
		result = PASS;
		break;
	}

    return result;
}


/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// launch your tests here

	//TEST_OUTPUT("divide_by_zero", div_z());
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("page_test_null", page_test_null());
	//TEST_OUTPUT("page_test_deref_out", page_test_deref_out());
	// TEST_OUTPUT("terminal_read", read_test());
	// printf("\n");
	// TEST_OUTPUT("terminal_write", write_test());
	// printf("\n");
	// TEST_OUTPUT("terminal_read_and_write", read_n_write_test());
	// printf("\n");
}
