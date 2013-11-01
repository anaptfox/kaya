#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/interupts.e"
#include "../e/scheduler.e"


state_t *sys_old = (state_t *) SYS_OLD;
state_t *pgm_old = (state_t *) PGMTRAP_OLD;
state_t *tlb_old = (state_t *) TLB_OLD;

void syshandler(){

	int cause;

	int kernel_mode;

	moveState(sys_old, &(currentProc->p_state));

	currentProc->s_pc = currentProc->s_pc + 4;

	kernal_mode = (sys_old->s_status) & //figure out if bit 3 is on;

	causeExcCode = sys_old->s_cause ;

	//Check for sys 
	if(causeExcCode == 8){
		//if user mode
		if (kernel_mode != 0 ){
			//check for sys 1-8
			if ((sys_old->reg_a0 > 0) && (sysBp_old->reg_a0 <= 8)){
				//moving the processor state from the SYS/Bp Old Area to the PgmTrap Old Area
				moveState(pgm_old, sys_old);

				//setting Cause.ExcCode in the PgmTrap Old Area to RI (Reserved Instruction)
				sys_old->s_cause = RI; //QUESTION  what is RI?

				// and calling Kaya’s PgmTrap exception handler.
				pgmTrapHandler();
			}else{
				//QUESTION: What to do here?
			}
		//if kernel mode
		}else{
			//check for sys 1-8
			if((sys_old->reg_a0 > 0) && (sysBp_old->reg_a0 <= 8)){
	
				switch(sys_old->reg_a0){
			        case CREATEPROCESS:
			            createProcess((state_t *) arg1);
			        break;
			                                
			        case TERMINATEPROCESS:        
			            terminateProcess();
			        break;
			        
			        case VERHOGEN:
			            Verhogen((int *) arg1);
			        break;
			        
			        case PASSEREN:
			            Passeren((int *)  arg1);
			        break;
			        
			        case EXCEPTION:
			        	handleSys5((int) arg1, (memaddr) arg2, (memaddr) arg3);
			        break;
			        
			        case GETCPUTIME:
			            getCpuTime();
			        break;
			        
			        case WAITFORCLOCK:
			            waitforclock();
			        break;
			        
			        case WAITFORIODEVICE:
			            waitForIO((int) arg1, (int) arg2, (int) arg3);
			        break;
			        
			        default:
			               //something
			    }
			// not sys 1-8
			}else{
				passUpOrDie();
			}	
		}
	//check for break
	}else if(causeExcCode == 9){
		// a0 contains LDST, FORK, PANIC, or HALT[1. . .4]
		if(notIssuedSys5){
			//Kill it
			terminateProcess(currentProc);
		}else{
			//The processor state is moved from the SYS/Bp Old Area into the processor
			// state area whose address was recorded in the ProcBlk as the SYS/Bp Old Area Address
			moveState(sys_old, &(currentProc->pcb_vect[3]->oldState));
			moveState(&(currentProc->pcb_vect[3]->newState, &(currentProc->p_state));

		}
	//Not sys or break panic;
	}else{
		PANIC();
	}
                  
}


void pgmTrapHandler(){

	if(notIssuedSys5){
			//Kill it
			terminateProcess(currentProc);
		}else{
			//The processor state is moved from the SYS/Bp Old Area into the processor
			// state area whose address was recorded in the ProcBlk as the SYS/Bp Old Area Address
			moveState(pgm_old, &(currentProc->pcb_vect[2]->oldState));
			moveState(&(currentProc->pcb_vect[2]->newState, &(currentProc->p_state))

		}
	}

}


void TLBHandler(){

	if(notIssuedSys5){
			//Kill it
			terminateProcess(currentProc);
		}else{
			//The processor state is moved from the SYS/Bp Old Area into the processor
			// state area whose address was recorded in the ProcBlk as the SYS/Bp Old Area Address
			moveState(tlb_old, &(currentProc->pcb_vect[0]->oldState));
			moveState(&(currentProc->pcb_vect[0]->newState, &(currentProc->p_state))

		}

}


void createProcess(state_t *state){
	pcb_t *newPcb;

	if((newPcb = allocPcb()) == NULL){
		
		state->s_v0 = -1;
		
	}else{
		
		processCnt++; // get a pcb, processcnt++)alloc
		
		newPcb->p_s = state->s_a1;// Copy the state pointed by a1 into the p_s of the new pct
		 	
		currentProc->currentProc_child = newPcb; // make the newpcb a child of current
		 	
		insertProcQ(&readyQue, newPcb);// put the new pcb  on the readyQue
		
		state->s_v0 = 0;
		
	}
	
}

//Syscall 2. killemAll should probably either go in terminateJob and is called here,
//or goes here and is called in terminateJob().
void terminateProcess(pct *p){
	 while(!emptychild(p)){
	 		terminateProcess(removeChild(p));
	 }
 	if( p == currentProc)
 		outChild(p);
 		currentProc = NULL; // can be done out side of kill em all. 
 	if (p->p_semAdd == null){
 		// on the ready que
 		outProcQ(&(readyQue), p);
 	}
 	else{
 		// on a sema4
 		if (outBlocked(p) != I/O){//QUESTION
 			Verhogen(p);
 		}else{
 			softBlkCnt--;
 		}	
 	}

}

//When this service (syscall 3) is requested, it is interpreted by the nucleus as a request to
//perform a V operation on a semaphore. (What is a V operation?)
void Verhogen(int *semaddr){

	pcb_t *p;

	*(semaddr)++;

	if(*(semaddr) <= 0){
		p = removeBlocked(semaddr);
		insertProcQ ( & readyQueue ,  p );
	}

	continueWithCurrent();
	
}
//When this service (syscall 4) is requested, it is interpreted by the nucleus as a request to
//perform a P operation on a semaphore. (What is a P operation?)
void Passeren(){
	
	pcb_t *p;

	semaddr--;

	if(semaddr <= -1){
		insertBlocked (semaddr , currentProc);
		//DO TIMING STUFF
		currentProc = NULL;
		scheduler();
	}
}

void getCpuTime(){
	currentProc->p_state.reg_v0 = currentProc->p_time;
	LDST(sys_old);
}

// This instruction performs a P operation on the nucleus maintained pseudo-clock
// timer semaphore. This semaphore is V’ed every 100 milliseconds automatically
// by the nucleus
void waitForClock(){

}

void waitForIO(){


}

void handleSys5((int) arg1, (memaddr) arg2, (memaddr) arg3){


	//save the contents of a2 and a3 (in the invoking process’es ProcBlk)
	pcb_vect = currentProc->p_states[arg1];

	moveState(arg2, &(pcb_vect->oldState));
	moveState(arg3, &(pcb_vect->newState));

	//the nucleus stores the processor state at the time of the exception in the area
	//pointed to by the address in a2
	moveState(&(currentProc->p_state), arg2);

	//and loads the new processor state from the area
	//pointed to by the address given in a3
	LDST(arg3);


}


