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

extern semd_t *deviceSemas[DEVICE_CNT][DEVICE_LINE];

extern semd_t *pseudo_clock;
 
extern cpu_t startTOD;

extern void moveState(state_t *before, state_t* after);
extern void test(void);
extern int main(void);
#endif
