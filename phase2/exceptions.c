void syshandler(){

	int cause;

	int kernal_mode;

	state_t *sys_old = (state_t *) SYS_OLD;

	// get the cause from the SYS old

	// ,, 2
	

	if (kernal_mode){
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

	//did the current job perform a sys 5 for pgmtraps?
	//if no, nuke it
	// if yes, 

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
	// while(!emptychild(p)){
	// 		killallofem(removeChild(p))
	// 	}
	// 	if( p == currentProc)
	// 		outChild(p);
	// 		currentProc = NULL; // can be done out side of kill em all. 
	// 	if (p->p_semAdd == null){
	// 		// on the ready que
	// 		outProcQ();
	// 	}
	// 	else
	// 		// on a sema4
	// 		outBlocked();
	// 		increment the sema4 by 1 
	// 			if it is not on an I/O sema4
	// 		decrement softBlkCnt
	// 			if it is an I/O sema4
    while(!emptychild(currentProc)){
        terminateProcess(removeChild(currentProc));     
    }
    currentProc = NULL;;

}

//When this service (syscall 3) is requested, it is interpreted by the nucleus as a request to
//perform a V operation on a semaphore. (What is a V operation?)
void Verhogen(int *semaddr){

	pcb_t *p;

	semaddr++;

	if(semaddr <= 0){
		p = removeBlocked(semaddr);
		insertProcQ ( & readyQueue ,  p );
	}
	
}
//When this service (syscall 4) is requested, it is interpreted by the nucleus as a request to
//perform a P operation on a semaphore. (What is a P operation?)
void Passeren(){
	
	pcb_t *p;

	semaddr--;

	if(semaddr <= -1){
		insertBlocked (semaddr , currentProc);
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

continueCurrentProcess()
