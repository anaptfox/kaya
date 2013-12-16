#ifndef INITIAL
#define INITIAL


#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "../e/interrupts.e"
#include "/usr/local/include/umps2/umps/libumps.e"


extern static pcb_t *readyQue;

extern static pcb_t *currentProc;

extern static int processCnt;

extern static int softBlkCnt;

extern static int deviceSemas[DEVICE_LINE][DEVICE_CNT];

extern static unsigned int deviceStatuses[DEVICE_LINE][DEVICE_CNT];

extern static int clockSem;
 
extern static cpu_t startTOD;

extern static cpu_t endTOD;

extern void moveState(state_t *before, state_t *after);
extern int main(void);
#endif
