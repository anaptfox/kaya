#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include <libumps.e>

void scheduler(){

	currentProc = removeProc(&readyque);

	if(currentProc == NULL){
		// If the Process Count is zero invoke the HALT ROM service/instruction.
		if(processCnt == 0){
			HALT();
		}else if(processCnt > 0 && softBlkCnt = 0){
		//Deadlock for Kaya is deﬁned as when the Process Count > 0 and the Soft-block Count is zero. 
			PANIC();
		}else{
		// if process count > 0 and the soft-block count > 0 enter a wait state. 
			//enable interrupts

			//handle interrupt
			WAIT();
		}
	}

	// DO TIMING STUFF
	LDIT(5000);
	STCK(startTOD);
	continueWithCurrent(&currentProc->p_s); 
	

}


void continueWithCurrent(state_t *process){
	LDST(process);
}
