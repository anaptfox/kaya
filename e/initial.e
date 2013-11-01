#ifndef INITIAL
#define INITIAL


#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "../e/interrupts.e"
#include <libumps.e>

extern void moveState(state_t *before, state_t* after);
extern void test(void);
extern int main(void);
#endif
