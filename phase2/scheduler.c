

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "libumps.e"


void scheduler(){

	

}

void createNewJob(){
	// get a pcb, processcnt++
	// Copy the state pointed by a1 into the p_s of the new pct
	 	
	// mae the newpcb a child of current
	 	
	// put the new pcb  on the readyQUe
	//LDSt(&oldsys)
}

void terminateJob(){
	// kill current job and all its progeny
	//recursivly travel tree and kill everything (freepcb)
	//everytime you call freepcb call procCnt--
}