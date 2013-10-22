
#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/initial.e"
#include <libumps.e>


void scheduler(){

	if(readyQue == NULL){
		if(processCnt == 0){
			HALT();
		}

		if(processCnt > 0 && softBlkCnt = 0){
			PANIC();
		}

		if(processCnt > 0 && softBlkCnt > 0){
			WAIT();
		}

	}

	currentProc = removeProc( &readyque)
	//put time on the clock 5000 micros.
	LDIT(5000);
	LDST(&currentProc->p_s); 

}

void createNewJob(){
	// get a pcb, processcnt++
	// Copy the state pointed by a1 into the p_s of the new pct
	 	
	// make the newpcb a child of current
	 	
	// put the new pcb  on the readyQue
	//LDST(&oldsys)
}

void terminateJob(){
	// kill current job and all its progeny
	//recursively travel tree and kill everything (freepcb)
	//everytime you call freepcb call procCnt--
}
