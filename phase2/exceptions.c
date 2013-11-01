void syshandler(){

	int cause;

	int kernel_mode;

	state_t *sys_old = (state_t *) SYS_OLD;

	// get the cause from the SYS old

	// ,, 2
	

	if (kernel_mode){
		switch(sys_old->reg_a0){
        case CREATEPROCESS:
   			//, an error code of -1 is placed/returned in the caller’s v0, otherwise, return the value 0 in the caller’s v0
            currentProc->p_state.reg_v0 = createProcess((state_t *) arg1);
        break;
                                
        case TERMINATEPROCESS:        
            currentProc->p_state.reg_v0 = terminateProcess((int) arg1);
        break;
        
        case VERHOGEN:
            Verhogen((int *) arg1);
        break;
        
        case PASSEREN:
            Passeren((int *)  arg1);
        break;
        
        case EXCEPTION:
        	handleSys5(int) arg1, (int) arg2, (int) arg3);
        break;
        
        case GETCPUTIME:
            currentProc->p_state.reg_v0 = getCpuTime();
        break;
        
        case WAITFORCLOCK:
            waitforclock();
        break;
        
        case WAITFORIODEVICE:
            currentProc->p_state.reg_v0 = waitForIO((int) arg1, (int) arg2, (int) arg3);
        break;
        
        default:
               //something
    }
	}
                        
    scheduler();

}


void pgmTrapHandler(){

	case EXCEPTION:
	if(p_state.reg_a1 != 0){
		terminateProcess; //note: the same thing as sys2 happens; not sure if this will work, or if we need to write a different kind of killEmAll
	}
	else{
		 pgmTrapOldAreaAddress = pgmOldArea;
		 currentProc = pgmTrapNewAreaAddress;//If the offending process has issued a SYS5 for PgmTrap exceptions, the
	//handling of the PgmTrap is “passed up.” The processor state is moved
	//from the PgmTrap Old Area into the processor state area whose address
	//was recorded in the ProcBlk as the PgmTrap Old Area Address. Finally, the
	//processor state whose address was recorded in the ProcBlk as the PgmTrap
	//New Area Address is made the current processor state.
	}

}


void TLBHandler(){

}


void createProcess( state_t *state){
	pcb_t *newPcb;
	if((newPcb = allocPcb()) == NULL){
		
		state->s_v0 = -1;
		
		return -1;

	}else{
		
		processCnt++; // get a pcb, processcnt++)alloc
		
		newPcb->p_s = state->s_a1;// Copy the state pointed by a1 into the p_s of the new pct
		 	
		currentProc->currentProc_child = newPcb; // make the newpcb a child of current
		 	
		insertProcQ(&readyQue, newPcb);// put the new pcb  on the readyQue
		
		state->s_v0 = 0;
		
		return 0;
		
	}
	
}

//Syscall 2. killemAll should probably either go in terminateJob and is called here,
//or goes here and is called in terminateJob().
void terminateProcess(){
	 while(!emptychild(p)){
	 		terminateProcess(removeChild(p));
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
	 		if (outBlocked(p) != I/O){
	 		Verhogen(p);
	 		}
	 		else{
	 		softBlkCnt--;
	 		}
    //while(!emptychild(currentProc)){
        //terminateProcess(removeChild(currentProc));     
    //}currentProc = NULL;

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
	return currentProc->p_time;
}

void waitforclock(){

}

void waitForIO(){

}

void handleSys5(){

}

