#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include <libumps.e>


void scheduler(){

	if(currentProc == NULL){

		currentProc = removeProc(&readyque);

		if(currentProc == NULL){
			// If the Process Count is zero invoke the HALT ROM service/instruction.
			if(processCnt == 0){
				HALT();
			}
			//Deadlock for Kaya is deﬁned as when the Process Count > 0 and the Soft-block Count is zero. 
			if(processCnt > 0 && softBlkCnt = 0){
				PANIC();
			}
			// if process count > 0 and the soft-block count > 0 enter a wait state. 
			if(processCnt > 0 && softBlkCnt > 0){
				WAIT();
				//handle interrupt
			}
		}

		// DO TIMEING STUFF
		LDIT(5000);
		startTtime(startTOD);
		LDST(&currentProc->p_s); 
	}

	//what if its not null?

}


void continueWithCurrent(){
	//continue with current job
}
