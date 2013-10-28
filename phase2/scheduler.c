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



	
	// kill current job and all its progeny
	//recursively travel tree and kill everything (freepcb)
	//everytime you call freepcb call procCnt--

