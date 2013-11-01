#ifndef EXCEPTIONS
#define EXCEPTIONS

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/interupts.e"
#include "../e/scheduler.e"

extern void syshandler();
extern void pgmTrapHandler();
extern void TLBHandler();
extern void createProcess(state_t *state);
extern void terminateProcess();
extern void Verhogen(int *semaddr);
extern void Passeren();
extern void getCpuTime();
extern void waitForClock();
extern void waitForIO();
extern void handleSys5();
extern void passUpOrDie();
#endif
