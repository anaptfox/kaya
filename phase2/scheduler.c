#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/asl.e"
#include "../e/initial.e"
#include "/usr/local/include/umps2/umps/libumps.e"

void debugProcess (state_t *p, int b, int c) {
  int foo = 42;
}

void scheduler(){

	currentProc = removeProcQ(&readyQue);


	if(currentProc == NULL){
		/*If the Process Count is zero invoke the HALT ROM service/instruction. */
		if(processCnt == 0){
			HALT();
		}else if(processCnt > 0 && softBlkCnt == 0){
		/*Deadlock for Kaya is deﬁned as when the Process Count > 0 and the Soft-block Count is zero. 
		*/	PANIC();
		}else{
		/*if process count > 0 and the soft-block count > 0 enter a wait state. 
			enable interrupts*/
		    LDIT(100000);
		    debugProcess(&(currentProc->p_s), 10, 10);
				setTIMER(0x08001101);
				debugProcess(&(currentProc->p_s), 10, 10);
				/*handle interrupt*/
				WAIT();
		}
	}

	LDIT(5000);
	STCK(startTOD);
	continueWithCurrent(&(currentProc->p_s)); 
	

}


void continueWithCurrent(state_t *process){
	debugProcess(process, 10, 10);
	LDST(process);
}
