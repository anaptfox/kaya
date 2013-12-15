#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/asl.e"
#include "../e/exceptions.e"
#include "../e/interrupts.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"
#include "../e/scheduler.e"

/* Global var */

pcb_t *readyQue;

pcb_t *currentProc;

int processCnt;

int softBlkCnt;

int deviceSemas[DEVICE_LINE][DEVICE_CNT];

unsigned int deviceStatuses[DEVICE_LINE][DEVICE_CNT];

int clockSem;

cpu_t startTOD;

int i;
void debugMikeyg (int a, int b, int c) {
  int foo = 42;
}

/* Copy before into after */
void moveState(state_t *before, state_t *after){
	after->s_asid = before->s_asid;
	after->s_cause = before->s_cause;
	after->s_status = before->s_status;
	after->s_pc = before->s_pc;
	i = 0;
	while(i < STATEREGNUM + 1) {
		after->s_reg[i] = before->s_reg[i];
		i = i + 1;
	}
}

extern void test(void);

int main(void)
{
		/* inilize the four new areas */
		/* set up devregarea*/
		devregarea_t *devregarea;

		debugMikeyg(10,10,10);

		devregarea = (devregarea_t *) 0x10000000;

		debugMikeyg(10,10,10);

		/* SYSCALLS*/
		state_t *area;
		area = (state_t *)SYS_NEW;
		STST(area);

		debugMikeyg(10,10,10);

		area->s_pc = area->s_t9 = (memaddr) sysHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		debugMikeyg(10,10,10);

		/* ProgramTrap*/
		area = (state_t *)PGMTRAP_NEW;
		moveState(area, (state_t *) PGMTRAP_NEW);
		area->s_pc = area->s_t9 = (memaddr) pgmTrapHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		debugMikeyg(10,10,10);

		/* TLB Management*/
		area = (state_t *)TLB_NEW;
		moveState(area, (state_t *) TLB_NEW);
		area->s_pc = area->s_t9 = (memaddr) TLBHandler ;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		debugMikeyg(10,10,10);

		/* Interrupt*/
		area = (state_t *)INT_NEW;
		moveState(area, (state_t *) INT_NEW);
		area->s_pc = area->s_t9 = (memaddr) intHandler;
		area->s_sp = devregarea->rambase + devregarea->ramsize;
		area->s_status = ALLOFF;

		debugMikeyg(10,10,10);

	

	initPcbs();

	debugMikeyg(10,10,10);
	
	initASL();

	debugMikeyg(10,10,10);

	LDIT(100000);

	debugMikeyg(10,10,10);

	clockSem = 0;

	debugMikeyg(10,10,10);

	setTIMER(5000);

	debugMikeyg(10,10,10);

	/* iniltialize semaphores to 0*/
	int j;
	
	for(i=0; i<DEVICE_LINE; i = i + 1){
  
    for(j=0; j<DEVICE_CNT; j = j + 1){
  
      deviceSemas[i][j] = 0;
  
    }
  
  }

  debugMikeyg(10,10,10);

	readyQue = mkEmptyProcQ();

	debugMikeyg(10,10,10);

	/* Alloc the first pcb */
	pcb_t *p = allocPcb();
	
	if(p == NULL){

		PANIC();
	
	}

	debugMikeyg(10,10,10);


	p->p_s.s_pc = p->p_s.s_t9 = (memaddr) test;

	debugMikeyg(10,10,10);

	p->p_s.s_sp = (devregarea->ramsize + devregarea->rambase) - PAGESIZE;

	debugMikeyg(10,10,10);
	
	p->p_s.s_status = p->p_s.s_status | 0x0800ff04;

	debugMikeyg(10,10,10);
	
	currentProc = NULL;

	debugMikeyg(10,10,10);
	
	processCnt = softBlkCnt = 0;

	debugMikeyg(10,10,10);

	/* populate this pcb*/
	insertProcQ(&readyQue, p);

	debugMikeyg(10,10,10);

	processCnt = processCnt + 1;

	debugMikeyg(10,10,10);

	
	scheduler();

}

