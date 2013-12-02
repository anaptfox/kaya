#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/interrupts.e"
#include "../e/scheduler.e"
#include "/usr/local/include/umps2/umps/libumps.e"


state_t *sys_old = (state_t *) SYS_OLD;
state_t *pgm_old = (state_t *) PGMTRAP_OLD;
state_t *tlb_old = (state_t *) TLB_OLD;

void debugA (int a, int b, int c) {
  int foo = 42;
}
void debugB (int a, int b, int c) {
  int foo = 42;
}
void debugC (int a, int b, int c) {
  int foo = 42;
}
void debugWAITFORIO (int a, int b, int c) {
  int foo = 42;
}

void sysHandler(){

	int kernel_mode;

	moveState(sys_old, &(currentProc->p_s));

	currentProc->p_s.s_pc = currentProc->p_s.s_pc + 4;
	
	kernel_mode = (sys_old->s_status) & KUp;


	/*if user mode */
	if (kernel_mode != 0 ){
		/*check for sys 1-8 */
		if ((sys_old->s_a0 > 0) && (sys_old->s_a0 <= 8)){

			/*setting Cause.ExcCode in the PgmTrap Old Area to RI (Reserved Instruction) */
			sys_old->s_cause = 10; /*QUESTION  what is RI?*/

			/*moving the processor state from the SYS/Bp Old Area to the PgmTrap Old Area */
			
			moveState(sys_old, pgm_old);

			/*and calling Kaya’s PgmTrap exception handler.*/
			pgmTrapHandler();

		}
	/*if kernel mode */
	}else{
		/*check for sys 1-8 */
		if((sys_old->s_a0 > 0) && (sys_old->s_a0 <= 8)){

			switch(sys_old->s_a0){

		        case CREATEPROCESS:
		            createProcess((state_t *) sys_old->s_a1);
		        break;
		                                
		        case TERMINATEPROCESS:        
		            terminateProcess(currentProc);
		        break;
		        
		        case VERHOGEN:
		            Verhogen((int *) sys_old->s_a1);
		        break;
		        
		        case PASSEREN:
		            Passeren((int *)  sys_old->s_a1);
		        break;
		        
		        case EXCEPTION:
		        	handleSys5((int) sys_old->s_a1, (memaddr) sys_old->s_a2, (memaddr) sys_old->s_a3);
		        break;
		        
		        case GETCPUTIME:
		            getCpuTime();
		        break;
		        
		        case WAITFORCLOCK:
		            waitForClock();
		        break;
		        
		        case WAITFORIODEVICE:
		            waitForIO((int) sys_old->s_a1, (int) sys_old->s_a2, (int) sys_old->s_a3);
		        break;
		        
		        default:
		            PANIC();
		    }
		/* not sys 1-8 */
		}else{
			if(currentProc->p_states[2].newState == NULL){
			/*Kill it */
			terminateProcess(currentProc);
			currentProc = NULL;
			scheduler();
			}else{
			/*The processor state is moved from the SYS/Bp Old Area into the processor
			state area whose address was recorded in the 
			ProcBlk as the SYS/Bp Old Area Address */
			moveState(pgm_old, (state_t *) currentProc->p_states[2].oldState);
			moveState((state_t *) currentProc->p_states[2].newState, &(currentProc->p_s));
			continueWithCurrent(&(currentProc->p_s));

		}
	}
		}	
	
                 
}

/*If sys5 returns a 1 in the a0, that is, we get a PgmTrap exception,
 pgmTrapHandler deals with the exception */
void pgmTrapHandler(){

	if(currentProc->p_states[1].newState == NULL){
			/*Kill it */
			terminateProcess(currentProc);
			currentProc = NULL;
			scheduler();
		}else{
			/*The processor state is moved from the SYS/Bp Old Area into the processor
			 state area whose address was recorded in the ProcBlk 
			as the SYS/Bp Old Area Address */
			moveState(pgm_old, (state_t *) currentProc->p_states[1].oldState);
			moveState((state_t *) currentProc->p_states[1].newState, &(currentProc->p_s));
			continueWithCurrent(&(currentProc->p_s));

		}

}

/* If sys5 returns a 0 in the a0, that is, we get a TLB exception,
TLBHandler deals with the exception */
void TLBHandler(){

	if(currentProc->p_states[0].newState == NULL){
			/*Kill it */
			terminateProcess(currentProc);
			currentProc = NULL;
			scheduler();
		}else{
			/*The processor state is moved from the SYS/Bp Old Area into the processor
			state area whose address was recorded in 
			the ProcBlk as the SYS/Bp Old Area Address */
			moveState(tlb_old, (state_t *) currentProc->p_states[0].oldState);
			moveState((state_t *) currentProc->p_states[0].newState, &(currentProc->p_s));

		}

}

/*When requested, this service causes a new process, said to be a progeny of the
caller, to be created. a1 should contain the physical address of a processor state
area at the time this instruction is executed. */
void createProcess(state_t *state){
	pcb_t *newPcb;

	if((newPcb = allocPcb()) == NULL){
		
		currentProc->p_s.s_v0 = -1;

		continueWithCurrent(&(currentProc->p_s));
		
	}else{
		
		processCnt++; /* get a pcb, processCnt++ alloc */
		
		
		moveState((state_t *)state->s_a1, &(newPcb->p_s));/*Copy the state pointed by a1 into the p_s of the new pct
		CALL MOVE STAE INSTEAT OF COPY */
		
		insertChild(currentProc, newPcb); /* make the newpcb a child of current
		INSERT CHILD */
		 	
		insertProcQ(&readyQue, newPcb);/* put the new pcb  on the readyQue */
		
		currentProc->p_s.s_v0 = 0;
		
		continueWithCurrent(&(currentProc->p_s));
	}
	
}

/*This services causes the executing process to cease to exist. In addition, recursively,
all progeny of this process are terminated as well. Execution of this instruction
does not complete until all progeny are terminated. */
void terminateProcess(pcb_t *p){
	 while(!emptyChild(p)){
	 		terminateProcess(removeChild(p));
	 }
 	if( p == currentProc)
 		outChild(p);
 		currentProc = NULL;  
 	if (p->p_semAdd == NULL){
 		/* on the ready que */
 		outProcQ(&(readyQue), p);
 	}
 	else{
 		/* on a sema4 */
 		p = outBlocked(p);
 		if ((&(p) > &(deviceSemas[0][0])) && (&(p) < &(deviceSemas[DEVICE_CNT][DEVICE_LINE]))){

 			*(p->p_semAdd)++;

 		}else{
 			softBlkCnt -= 1;
 		}	
 	}
 	freePcb(p);
 	processCnt  -= 1;

}

/*When this service (syscall 3) is requested, it is interpreted by the nucleus as a request to
perform a V operation on a semaphore.  */
void Verhogen(int *semaddr){

	pcb_t *p;

	*(semaddr) += 1;

	if(*(semaddr) <= 0){
		p = removeBlocked(semaddr);
		insertProcQ (&readyQue, p);
	}

	continueWithCurrent(&(currentProc->p_s));
	
}

/*When this service (syscall 4) is requested, it is interpreted by the nucleus as a request to
perform a P operation on a semaphore. */
void Passeren(int *semaddr){

	cpu_t endTOD;

	*(semaddr) -= 1;

	if(*(semaddr) <= -1){
		
		insertBlocked (semaddr , currentProc);
		
		STCK(endTOD);
	
		currentProc->p_time = endTOD - startTOD;
	
		currentProc = NULL;
		
		scheduler();
	}

	continueWithCurrent(&(currentProc->p_s));

}

void getCpuTime(){
	currentProc->p_s.s_v0 = currentProc->p_time;
	continueWithCurrent(&(currentProc->p_s));
}

/*This instruction performs a P operation on the nucleus maintained pseudo-clock
timer semaphore. This semaphore is V’ed every 100 milliseconds automatically
by the nucleus (use local timer) */
void waitForClock(){
	
	*(pseudo_clock->s_semAdd) -= 1;

	if(*(pseudo_clock->s_semAdd) <= -1){
		insertBlocked (pseudo_clock->s_semAdd , currentProc);
		/*DO TIMING STUFF
		store clock - do substracti - add to field of pcb */
		currentProc = NULL;
		scheduler();
	}
	softBlkCnt++;
	continueWithCurrent(&(currentProc->p_s));

}

void waitForIO(int arg1, int arg2, int terminalRead){

	/*arg1 = line number
	arg2 = device number
	arg3 = r/w
	*/

	/* If there is a terminal read increment device number by one to make a terminal transmition*/
	if(terminalRead){
		
		/* do nothing*/
	
	}else{
		
		arg1 = arg1 + 1;
		
		debugWAITFORIO(arg1,10,10);
	}

	cpu_t endTOD;

	deviceSemas[arg2][arg1] -= 1;

	debugA(arg1,10,10);

	if(deviceSemas[arg2][arg1] < 0){
		debugB(arg1,10,10);

		insertBlocked (&(deviceSemas[arg2][arg1] ), currentProc);

		STCK(endTOD);

		currentProc->p_time = endTOD - startTOD;

		currentProc = NULL;
		
		softBlkCnt++;
		
		scheduler();
		debugB(arg1,10,10);
	}

	debugA(arg1,10,10);
	if(terminalRead){
		
		currentProc->p_s.s_v0 = deviceStatuses[arg1 - 3][arg2];
	
	}else{
		
		currentProc->p_s.s_v0 = deviceStatuses[arg1 - 3 + 1][arg2];
	}

	debugA(arg1,10,10);
	
	continueWithCurrent(&(currentProc->p_s));


}


void handleSys5(int arg1, memaddr arg2, memaddr arg3){


	/*save the contents of a2 and a3 (in the invoking process's ProcBlk) */
	pcb_vect p_states = currentProc->p_states[arg1];

	p_states.oldState = arg2;
	p_states.newState = arg3;


	/*the nucleus stores the processor state at the time of the exception in the area
	pointed to by the address in a2 */
	arg2 = &(currentProc->p_s);

	/*and loads the new processor state from the area
	pointed to by the address given in a3 */
	LDST(&(arg3));


}


