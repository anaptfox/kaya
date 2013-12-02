#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/asl.e"
#include "../e/initial.e"
#include "/usr/local/include/umps2/umps/libumps.e"

void debugProcess (state_t *p, int b, int c) {
  int foo = 42;
}

void debugE (int a, int b, int c) {
  int foo = 42;
}
void debugF (int a, int b, int c) {
  int foo = 42;
}
void debugG (int a, int b, int c) {
  int foo = 42;
}

void debugWAIT (int a, int b, int c) {
  int foo = 42;
}

state_t *int_new = (state_t *) INT_NEW;
state_t *int_old_area = (state_t *) INT_OLD;


void scheduler(){

	if(currentProc == NULL){
		/*If the Process Count is zero invoke the HALT ROM service/instruction. */
		if(processCnt == 0){
			HALT();
		}else if(processCnt > 0 && softBlkCnt == 0){
			debugG(10,10,10);
		/*Deadlock for Kaya is deﬁned as when the Process Count > 0 and the Soft-block Count is zero. 
		*/	PANIC();
		}else{
		/*if process count > 0 and the soft-block count > 0 enter a wait state. 
			enable interrupts*/
				debugE(10,10,10);
		    LDIT(100000);
		    debugE(10,10,10);
				setTIMER(0x08001101);
				debugE(10,10,10);
				/*handle interrupt*/
				WAIT();
	
		}
	}

	currentProc = removeProcQ(&readyQue);

	debugG(10,10,10);
	
	continueWithCurrent(&(currentProc->p_s)); 
	

}


void continueWithCurrent(state_t *process){
	debugF(10,10,10);
	LDST(process);
}
