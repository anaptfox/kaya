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
	newPcb = allocPcb();
	processCnt++; // get a pcb, processcnt++)alloc
	newPcb->p_s = *a1;// Copy the state pointed by a1 into the p_s of the new pct
	 	
	currentProc->currentProc_child = newPcb;// make the newpcb a child of current
	 	
	insertProcQ(readyQue->tp newPcb->p_s);// put the new pcb  on the readyQue
	LDST(&oldsys)
}

void terminateJob(p){
                while(!emptychild(p)){
                       terminateJob(removeChild(p))
                }
                if( p == currentProc)
                        outChild(p);
                        currentProc = NULL; // can be done out side of kill em all. 
                if (p->p_semAdd == null){
                        // on the ready que
                        outProcQ();
                }
                else
                        // on a sema4
                        outBlocked();
                        increment the sema4 by 1 
                                if it is not on an I/O sema4
                        decrement softBlkCnt
                                if it is an I/O sema4


                freePcb(p)
                procCnt -= 1;
	}
	
	// kill current job and all its progeny
	//recursively travel tree and kill everything (freepcb)
	//everytime you call freepcb call procCnt--

