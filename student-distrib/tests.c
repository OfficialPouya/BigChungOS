#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"

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
	//TEST_OUTPUT("rtc_test", rtc_test());
	//TEST_OUTPUT("rtc_bad_input_test1", rtc_bad_input_test1());
	//TEST_OUTPUT("rtc_bad_input_test2", rtc_bad_input_test2());
	//TEST_OUTPUT("rtc_bad_input_test3", rtc_bad_input_test3());
}
