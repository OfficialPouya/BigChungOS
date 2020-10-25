#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
<<<<<<< HEAD
#include "rtc.h"

=======
#include "keyboard.h"
#include "terminal.h"
>>>>>>> c2d299c40def61e9b20c57fa30febb010ee99e8e
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

/* Checkpoint 2 tests */
/*
 NAME: rtc_test
 DESCRIPTION: tests rtc at every frequency 2-1024
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: Should return PASS
 IMPACTS ON OTHERS: none
 */
int rtc_test(){
	uint32_t freq; 
	int i; 
	clear();
	rtc_open(0);
	for(freq = 2; freq <= 1024; freq *= 2){
		for(i = 0; i < 15; i++){
			rtc_write(0, (void*)&freq, 4);
			rtc_read(0, 0, 4);
			printf("1");
		}
		printf("\n");
		clear();
	}
	rtc_close(0);
	return PASS;
}
/*
 NAME: rtc_bad_input_test1
 DESCRIPTION: tests bad input for frequency (frequency > 1024), should cause RTC to not do anything. 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: Should return PASS
 IMPACTS ON OTHERS: none
 */
int rtc_bad_input_test1(){
	TEST_HEADER;
	uint32_t freq = 2048;
	rtc_open(0);
	rtc_write(0, (void*)&freq, 4);
	rtc_close(0);
	return PASS;
}
/*
 NAME: rtc_bad_input_test2
 DESCRIPTION: tests bad input for buffer pointer, should cause RTC to not do anything. 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: Should return PASS
 IMPACTS ON OTHERS: none
 */
int rtc_bad_input_test2(){
	TEST_HEADER;
	rtc_open(0);
	rtc_write(0,NULL,4);
	rtc_close(0);
	return PASS;
}
/*
 NAME: rtc_bad_input_test3
 DESCRIPTION: tests bad input for nbytes, should cause RTC to not do anything. 
 INPUTS: NONE
 OUTPUTS: NONE
 RETURN VALUE: Should return PASS
 IMPACTS ON OTHERS: none
 */
int rtc_bad_input_test3(){
	TEST_HEADER;
	uint32_t freq = 2; 
	rtc_open(0);
	rtc_write(0,(void*)&freq,3);
	rtc_close(0);
	return PASS;
}

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



/* Check if can open, close, and write files 
*
* Should print pcb before and after open/close
* write should print -1, as expected 
*
* Inputs: None
* Outputs: Should display contents of primitive pcb
* Side Effects: None
* Coverage: file system
* Files: FileSystem.c/.h 
*/
void open_close_write_test() {
	int i;
	uint8_t* temp;
	temp = (uint8_t*)"frame1.txt";
	
	clear();
	printf("Before fopen: ");
	for(i = 0; i < 8; i++){printf("%d, ", inode_list[i].inode_num);}
	file_open(temp);
	printf("\nAfter fopen: ");
	for(i = 0; i < 8; i++){printf("%d, ", inode_list[i].inode_num);}
	printf("\nResult of fwrite: %d", file_write());
	printf("\nAfter fclose: ");
	file_close(temp);
	for(i = 0; i < 8; i++){printf("%d, ", inode_list[i].inode_num);}
}

/* Home Brew LS 
*
* Should print filenames of all files in file system
*
* Inputs: None
* Outputs: All 17 items in file system
* Side Effects: None
* Coverage: file system
* Files: FileSystem.c/.h 
*/
void dir_read_test() {
	clear();
	while(dir_read()==0);
}

/* File read test
*
* Should print data for a file
*
* Inputs: filename to output
* Outputs: file data
* Side Effects: None
* Coverage: file system
* Files: FileSystem.c/.h 
*/
void file_read_test() {
	uint8_t temper[5280];			// 5280 is the size of the largest amount of bytes we need to read for this checkpoint
	uint8_t* temp;					// number is based off of verylargetextwithverylongname.tx
	
	//temp = (uint8_t*)"frame0.txt";
	//temp = (uint8_t*)"frame1.txt";
	temp = (uint8_t*)"grep";
	//temp = (uint8_t*)"ls";
	//temp = (uint8_t*)"fish";
	//temp = (uint8_t*)"verylargetextwithverylongname.tx";

	clear();
	file_open(temp);
	file_read(temp, temper);
	
	int i;
	for (i = 0; i < bytes_read; i++){
		putc(temper[i]);
	}
}
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// launch your tests here
	//open_close_write_test();
	//dir_read_test();
	file_read_test();
	//TEST_OUTPUT("divide_by_zero", div_z());
	//TEST_OUTPUT("divide_by_zero", div_z());
	//TEST_OUTPUT("idt_test", idt_test());
	//TEST_OUTPUT("page_test_null", page_test_null());
	//TEST_OUTPUT("page_test_deref_out", page_test_deref_out());
<<<<<<< HEAD
	//TEST_OUTPUT("rtc_test", rtc_test());
	//TEST_OUTPUT("rtc_bad_input_test1", rtc_bad_input_test1());
	//TEST_OUTPUT("rtc_bad_input_test2", rtc_bad_input_test2());
	//TEST_OUTPUT("rtc_bad_input_test3", rtc_bad_input_test3());
=======
	// TEST_OUTPUT("terminal_read", read_test());
	// printf("\n");
	// TEST_OUTPUT("terminal_write", write_test());
	// printf("\n");
	// TEST_OUTPUT("terminal_read_and_write", read_n_write_test());
	// printf("\n");
>>>>>>> c2d299c40def61e9b20c57fa30febb010ee99e8e
}
