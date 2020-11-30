/* memory.h - This initializes the paging structures.
 * vim:ts=4 noexpandtab
 */
#pragma once

#include "types.h"
#include "x86_desc.h"

// These are used to set certain bits in entries
#define P       0x1     // Present bit
#define RW      0x2     // Read Write bit
#define US      0x4     // User/Supervisor bit
#define PCD     0x10    // Page Cache Disabled
#define PS      0x80    // Page size bit
#define G       0x100   // Global bit

// Shift amounts for addresses in either entries
#define ENTRY4KB    12  // Allows access to top 20 bits
#define ENTRY4MB    22  // Allows access to top 10 bits

// Save the kernel addr and video memory addr
#define KERNELPG    0x400000
#define USERPG      0x800000
#define VIDMEM      0xB8000
#define TERM0       0xB9000
#define TERM1       0xBA000
#define TERM2       0xBB000

/* Enabe paging */
void paging_init(void);

/* quick function to call to flush TLB */
void flush_tlb(void);

/* enter process # of desired program space */
void update_user_addr(int process_num);
