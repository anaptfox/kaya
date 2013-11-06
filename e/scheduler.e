#ifndef SCHEDULER
#define SCHEDULER

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include "/usr/local/include/umps2/umps/libumps.e"

extern void scheduler();
extern void continueWithCurrent(state_t *process);
#endif
