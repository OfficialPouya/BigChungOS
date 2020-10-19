/* memory.c - This initializes the paging structures.
 * vim:ts=4 noexpandtab
 */

#include "memory.h"
#include "lib.h"

/*
 NAME: paging_init
 DESCRIPTION: This sets up all the bits in the already
                declared paging structures. It also 
                access cro0, cr3, and cr4 to enable paging
                and related features.
 IO: NONE
 IMPACTS ON OTHERS: Enables paging
 */
void paging_init(void) {
    int i;
    // This sets up entries of page directory entries
    for (i = 0; i < COMMON_SIZE; i++){
        page_directory[i] = 0;
        // First directory entry gets mapped to 4KB table
        // Requires: Present, Read/Write, and address bits
        if(i == 0){
            page_directory[i] |= P;
            page_directory[i] |= RW;
            page_directory[i] |= ((((uint32_t)(&page_table1)) >> ENTRY4KB) << ENTRY4KB);
        }
        // Second directory entry gets mapped to 4MB page reserved for kernel
        // Requires: Present, Read/Write, Page cache, page size, global, and address bits
        if(i == 1){
            page_directory[i] |= P;
            page_directory[i] |= RW;
            page_directory[i] |= PCD;
            page_directory[i] |= PS;
            page_directory[i] |= G;
            page_directory[i] |= KERNELPG;
        }
        // These are 1022 empty directories blanked, all pages in MP3 will be RW enabled
        else {
            page_directory[i] |= RW;
        }
    }

    // This sets up entries of first table
    for (i = 0; i < COMMON_SIZE; i++){
        page_table1[i] = 0;
        // This speciifc table entry gets mapped to 0xB8000 for vid mem
        // Requires: Present, Read/Write, and address bits
        if(i == (VIDMEM>>ENTRY4KB)){
            page_table1[i] |= P;
            page_table1[i] |= RW;
            page_table1[i] |= VIDMEM;
        }

        // These are 1022 empty table entries blanked, all pages in MP3 will be RW enabled
        else {
            page_table1[i] |= RW;
        }
    }
    
    /* 
    Setting control registers to enable paging.
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    $$$       ORDER IS SO IMPORTANT HERE     $$$
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    Assume, CR0 has paging disabled when starting
    First, Enable PSE, or Page Size Extension
    to enable 4MB pages.
    Second, Load address of page directory into 
    CR3
    Third, Enable paging, leave in real mode.
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    $$$       ORDER IS SO IMPORTANT HERE     $$$
    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    */
    asm volatile (
            "movl %%cr4, %%eax          /* move cr4 to EAX*/                     ;"
            "orl  $0x10, %%eax          /* 0x10 = 5th bit, enables 4MB*/         ;"
            "movl %%eax, %%cr4          /* move changed value back to cr4*/      ;"
            "lea page_directory, %%eax  /* grab address of pd*/                  ;"
            "movl %%eax, %%cr3          /* place into cr3*/                      ;"
            "movl %%cr0, %%eax          /* move cr0 to EAX*/                     ;"
            "orl $0x80000000, %%eax     /* 0x80000000 = 32nd bit enables paging*/;"
            "movl %%eax, %%cr0                                                   "
        : // no outputs 
        : // no inputs 
        : "eax"
    );
}
