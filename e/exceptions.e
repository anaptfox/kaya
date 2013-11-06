#ifndef EXCEPTIONS
#define EXCEPTIONS

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/interrupts.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"

extern void sysHandler();
extern void pgmTrapHandler();
extern void TLBHandler();
extern void createProcess(state_t *state);
extern void terminateProcess(pcb_t *p);
extern void Verhogen(int *semaddr);
extern void Passeren(int *semaddr);
extern void getCpuTime();
extern void waitForClock();
extern void waitForIO(int arg1, int arg2, int arg3);
extern void handleSys5(int arg1, memaddr arg2, memaddr arg3);
#endif
