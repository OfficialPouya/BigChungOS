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

        // 33rd directory entry gets mapped to 128MB page reserved for user programs
        // Requires: Present, Read/Write, Page cache, page size, global, and address bits
        if(i == 32){
            page_directory[i] |= P;
            page_directory[i] |= RW;
            page_directory[i] |= US;
            page_directory[i] |= PCD;
            page_directory[i] |= PS;
            page_directory[i] |= USERPG;
        }

        // Directory entry after user page gets mapped to 4KB table for user vidmem
        // Requires: Present, Read/Write, user, and address bits
        if(i == 33){
            page_directory[i] |= P;
            page_directory[i] |= RW;
            page_directory[i] |= US;
            page_directory[i] |= ((((uint32_t)(&page_table2)) >> ENTRY4KB) << ENTRY4KB);
        }
        // These are 1020 empty directories blanked, all pages in MP3 will be RW enabled
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

        if(i == (TERM0>>ENTRY4KB)){
            page_table1[i] |= P;
            page_table1[i] |= RW;
            page_table1[i] |= TERM0;
        }

        if(i == (TERM1>>ENTRY4KB)){
            page_table1[i] |= P;
            page_table1[i] |= RW;
            page_table1[i] |= TERM1;
        }

        if(i == (TERM2>>ENTRY4KB)){
            page_table1[i] |= P;
            page_table1[i] |= RW;
            page_table1[i] |= TERM2;
        }
        // These are 1022 empty table entries blanked, all pages in MP3 will be RW enabled
        else {
            page_table1[i] |= RW;
        }
    }

    // This sets up entries of second table
    for (i = 0; i < COMMON_SIZE; i++){
        page_table2[i] = 0;
        // This speciifc table entry gets mapped to 0xB8000 for vid mem
        // Requires: Present, Read/Write, and address bits
        if(i == (VIDMEM>>ENTRY4KB)){
            page_table2[i] |= P;
            page_table2[i] |= RW;
            page_table2[i] |= US;
            page_table2[i] |= VIDMEM;
        }

        // These are 1022 empty table entries blanked, all pages in MP3 will be RW enabled
        else {
            page_table2[i] |= RW;
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
    Third, Enable paging, leave in protected mode.
    If you enable paging in real mode you'll
    encounter page fault.
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

/*
 NAME: flush_tlb
 DESCRIPTION: This simply flushes the TLB by
              putting same directory back into
              cr3.
 IO: NONE
 IMPACTS ON OTHERS: Flushes TLB
 */
void flush_tlb(void) {
    /* gets address of same directory,
       and puts value back in cr3.
    */
    asm volatile (
            "lea page_directory, %%eax  /* grab address of pd*/                  ;"
            "movl %%eax, %%cr3          /* place into cr3*/                       "
        : // no outputs
        : // no inputs
        : "eax"
    );
}

/*
 NAME: update_user_addr
 DESCRIPTION: This will update the 128MB adress based on process num
              Process 0 (SHELL), will point to 8MB.
              Subsequent processes will be 8 + 4*process#
              At end, will flush TLB, to reflect change to directory.
 IO: proc number
 IMPACTS ON OTHERS: Changes address pointed by 128MB page
 */
void update_user_addr(int process_num){
    // 32 is the index for the user program page
    page_directory[32] &= 0xFFFFF;      // Save all lower 20 bits
    page_directory[32] |= (USERPG+(KERNELPG*process_num));
    flush_tlb();
}
