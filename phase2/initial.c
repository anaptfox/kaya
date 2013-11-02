#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "../e/interrupts.e"
#include "/usr/local/include/umps2/umps/libumps.e"

/* Global var */

pct_t *readyQue;

pcb_t *currentProc;

int processCnt;

int softBlkCnt;

semd_t *deviceSemas[DEVICE_CNT][DEVICE_LINE];

semd_t *pseudo_clock;

cpu_t startTOD;


/* Copy before into after */
void moveState(state_t *before, state_t *after){
	after->s_aside = before->s_aside;
	after->s_cause = before->s_cause;
	after->s_status = before->s_status;
	after->s_pc = before->s_pc;
	int i = 0;
	while(i < STATEREGNUM + 1) {
		after->s_reg[i] = before->s_reg[i];
		i++;
	}
}

void test(void){

}

int main(void)
{
	/* inilize the four new areas */

		/* set up devregarea*/
		devregarea_t *devregarea;

		devregarea = (devregarea_t *) 0x10000000;

		/* SYSCALLS*/
		state_t *area;
		area = (state_t *)SYS_NEW;
		STST(area);

		area->s_pc = area->s_t9 = (memaddr) sysHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->status = ALLOFF;

		/* ProgramTrap*/
		area = (state_t *)PGMTRAP_NEW;
		moveState(area, (state_t *) PGMTRAP_NEW)
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;

		/* TLB Management*/
		area = (state_t *)TLB_NEW;
		moveState(area, (state_t *) TLB_NEW)
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;

		/* Interrupt*/
		area = (state_t *)INT_NEW;
		moveState(area, (state_t *) INT_NEW)
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;
	
	

	initPcbs();
	initASLs();

	/* iniltialize semaphores to 0*/

	for(int i=0; i<DEVICE_CNT; i++){
        for(int j=0; j<DEVICE_LINE; j++){
                  deviceSemas[i][j] = 0;
        }
    }


	/* Alloc the first pcb*/
	pct_t *p = allocPcb();
	if(p == NULL){
		PANIC();
	}
	p->s_pc = p->s_t9 = (memaddr) test;
	p->s_sp = RAMTOP - FRAMESIZE;
	p->status = 0x0800ff01; /* TODO: interrupts enabled, virtual memory off, processor local timer enables, kernel mode on*/
	mkEmptyProcQ(&readyQue);
	currentProc = NULL;
	processCnt = softBlkCnt = 0;

	/* populate this pcb*/
	insertProcQ(&readyQue, p);
	processCnt++;
	
	scheduler();

}

