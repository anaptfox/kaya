#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"

/* Global var */

pcb_t *readyQue;

pcb_t *currentProc;

int processCnt;

int softBlkCnt;

semd_t *deviceSemas[DEVICE_CNT][DEVICE_LINE];

semd_t *pseudo_clock;

cpu_t startTOD;

int i;

/* Copy before into after */
void moveState(state_t *before, state_t *after){
	after->s_asid = before->s_asid;
	after->s_cause = before->s_cause;
	after->s_status = before->s_status;
	after->s_pc = before->s_pc;
	i = 0;
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
		area->s_status = ALLOFF;

		/* ProgramTrap*/
		area = (state_t *)PGMTRAP_NEW;
		moveState(area, (state_t *) PGMTRAP_NEW);
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		/* TLB Management*/
		area = (state_t *)TLB_NEW;
		moveState(area, (state_t *) TLB_NEW);
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		/* Interrupt*/
		area = (state_t *)INT_NEW;
		moveState(area, (state_t *) INT_NEW);
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;
	
	

	initPcbs();
	initASLs();

	/* iniltialize semaphores to 0*/
	int j;
	for(i=0; i<DEVICE_CNT; i++){
        for(j=0; j<DEVICE_LINE; j++){
                  deviceSemas[i][j] = 0;
        }
    }


	/* Alloc the first pcb*/
	pcb_t *p = allocPcb();
	if(p == NULL){
		PANIC();
	}
	p->p_s.s_pc = p->p_s.s_t9 = (memaddr) test;
	p->p_s.s_sp = devregarea->ramsize - PAGESIZE;
	p->p_s.s_status = p->p_s.s_status | 0x0800ff01; /* TODO: interrupts enabled, virtual memory off, processor local timer enables, kernel mode on*/
	mkEmptyProcQ(&readyQue);
	currentProc = NULL;
	processCnt = softBlkCnt = 0;

	/* populate this pcb*/
	insertProcQ(&readyQue, p);
	processCnt++;
	
	scheduler();

}

