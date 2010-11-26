#ifndef _MEMORY_LAYOUT_H_
#define _MEMORY_LAYOUT_H_

#include "rtx.h"
#include "k_structs.h"
#include "k_globals.h"

#define MEM_START ((void *)&__end)
#define PCB_START (MEM_START)
#define PCB_END   (PCB_START+NUM_PROCESSES*sizeof(pcb_t))
#define PCB_CONTEXT_OFFSET 0

#endif
