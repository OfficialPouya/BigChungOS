#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

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
	dir_read();
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
	uint8_t* temper;
	uint8_t* temp;
	temp = (uint8_t*)"grep";

	clear();
	file_open(temp);
	file_read(temp, temper);
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


}
