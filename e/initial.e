#ifndef INITIAL
#define INITIAL


#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "../e/interrupts.e"
#include "/usr/local/include/umps2/umps/libumps.e"


extern pcb_t *readyQue;

extern pcb_t *currentProc;

extern int processCnt;

extern int softBlkCnt;

extern int deviceSemas[DEVICE_LINE][DEVICE_CNT];

extern unsigned int deviceStatuses[DEVICE_LINE][DEVICE_CNT];

extern semd_t *pseudo_clock;
 
extern cpu_t startTOD;

extern void moveState(state_t *before, state_t *after);
extern int main(void);
#endif
