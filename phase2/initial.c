#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"
#include "../e/interrupts.e"
#include <libumps.e>

// Global var

pct_t* readyQue;

pcb_t* currentProc;

int processCnt;

int softBlkCnt;

int deviceSemas[DEVICE_CNT][DEVICE_LINE];

cpu_t startTOD;


void moveState(state_t *before, state_t* after){
	// typedef struct state_t {
		// unsigned int	s_asid;
		// unsigned int	s_cause;
		// unsigned int	s_status;
		// unsigned int 	s_pc;
		// int	 			s_reg[STATEREGNUM];

	// } state_t, *state_PTR;
}

void test(void){

}

int main(void)
{
	state_t *area; 
	//inilize the four new area
		//SYSCALLS
		area = SYS_NEW;
		STST(area);

		area->s_pc = (memaddr) sysHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;

		//PRogramTrp
		moveState(area, (state_t *) PGMTRAP_NEW)
		area->s_pc = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;

		//TLB Management

		moveState(area, (state_t *) TLB_NEW)
		area->s_pc = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;

		//Interrupt

		moveState(area, (state_t *) INT_NEW)
		area->s_pc = (memaddr) pgmTrapHandler;
		area->s_sp = RAMBASEADDR - RAMTOP;
		area->status = ALLOFF;
	
	

	initPcbs();
	initASLs();

	//inilize semaphores to 0

	for(int i=0; i<DEVICE_CNT; i++){
        for(int j=0; j<DEVICE_LINE; j++){
                  deviceSemas[i][j] = 0;
        }
    }


	//initilize the pcbs
	pct_t *p = allocPcb();
	if(p == NULL){
		PANIC();
	}
	p->s_pc = (memaddr) test;
	p->s_sp = FRAMESIZE - RAMBASEADDR - RAMTOP;
	p->status = ALLOFF;
	mkEmptyProcQ(&readyQue);
	currentProc = NULL;
	processCnt = softBlkCnt = 0;

	//populate this pcb
	insertProcQ(&readyQue, p);
	processCnt++;
	
	scheduler();

}

